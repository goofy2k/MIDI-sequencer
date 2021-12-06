/* implementation of class MidiOutNimBLE */
/*
* FCKX december 2021
*/
#include "esp_log.h"
#include "nimBLEdriver.h" //make driver globally accessible by including this header file
#include <NimBLEDevice.h>

static const char *TAG = "NIMBLEDRIVER";

/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara
    Refactored back to IDF by H2zero

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
***********************/

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

//MIDI data packet, taken from Brian Duhan arduino-esp32-BLE-MIDI / BLE_MIDI.ino
uint8_t midiPacket[] = {
    0x80, //header
    0x80, //timestamp, not implemented
    0x00, //status
    0x3c, //== 60 == middle c
    0x00  //velocity
};




// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */  
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising(); //keep advertising to find more connections
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
/***************** New - Security handled here ********************
****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest(){
    printf("Server PassKeyRequest\n");
    return 123456; 
  }

  bool onConfirmPIN(uint32_t pass_key){
    printf("The passkey YES/NO number: %d\n", pass_key);
    return true; 
  }

  void onAuthenticationComplete(ble_gap_conn_desc desc){
    printf("Starting BLE work!\n");
  }
/*******************************************************************/
};


struct nimBLEMidiData {
  BLEServer* client;   //this "client" is actually a nimBLE server
                       //note that devices can have both roles concurrently
                       //after making a connection
                       //more important: the difference between Central and Peripheral
                       //this device is .....(?)   It takes the lead in making connections
                       //it starts "advertising"
  //clienName ......                     
  //nimBLE_port_t *port;
 // bool connected_; //in RtMidi, this is in the parent class
  //nimBLE_ringbuffer_t *buffSize;     //required for MidiIn
  //nimBLE_ringbuffer_t *buffMessage;  //required for MidiIn
  //nimBLE_time_t lastTime;            //???
#ifdef HAVE_SEMAPHORE                  //depends on the OS
  sem_t sem_cleanup;
  sem_t sem_needpost;
#endif
  //MidiInApi :: RtMidiInData *rtMidiIn; //???
  };

 MidiOutNimBLE :: MidiOutNimBLE ()
//MidiOutNimBLE :: MidiOutNimBLE (const std::string &clientName)
{   
    MidiOutNimBLE::initialize("fckx_seq2"); 
    //MidiOutNimBLE::initialize(clientName);    
}

MidiOutNimBLE :: ~MidiOutNimBLE ()
{
    //close a connection if it exists
    MidiOutNimBLE::closePort();
    
    //Cleanup
    //what does this comprise for nimBLEDevice ?
    
}

void MidiOutNimBLE :: initialize ( const std::string& clientName)
{
  // Set up our client and give a sign of life
  ESP_LOGW(TAG, "Initialize nimBLEOutdriver"); 
  BLEServer* pServer = NULL;  //must be made accessible for the outside world later
BLECharacteristic* pCharacteristic = NULL; 
  //Create the BLE Device 
  //This is also for Midi input, so should ideally be in a super class
  BLEDevice::init("fckx_seq"); //later use NimBLEDevice, and use cleintName to pass the name
  //NimBLEDevice::init("fckx_seq"); //later use NimBLEDevice, and use cleintName to pass the name
  //with or without the Nim:  this doesn't make a difference for the silent crash
  
  ESP_LOGW(TAG, "BLEDevice created"); 
#define TEMP_BLOCK1 1  
 #ifdef TEMP_BLOCK1 
  pServer = BLEDevice::createServer();
     ESP_LOGW(TAG, "BLE server created"); 
  pServer->setCallbacks(new MyServerCallbacks());
   ESP_LOGW(TAG, "BLE server callbacks created"); 
   
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
ESP_LOGW(TAG, "BLE server service created");
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                /******* Enum Type NIMBLE_PROPERTY now *******     
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
                **********************************************/    
                      NIMBLE_PROPERTY::READ   |
                      NIMBLE_PROPERTY::WRITE  |
                      NIMBLE_PROPERTY::NOTIFY |
                      NIMBLE_PROPERTY::INDICATE
                    );
ESP_LOGW(TAG, "BLE server characteristic created");
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  /***************************************************   
   NOTE: DO NOT create a 2902 descriptor. 
   it will be created automatically if notifications 
   or indications are enabled on a characteristic.
   
   pCharacteristic->addDescriptor(new BLE2902());
  ****************************************************/
  // Start the service
  pService->start();
ESP_LOGW(TAG, "BLE service started");
  // Start advertising
  ESP_LOGI(TAG, "Start advertising");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);                             
  BLEDevice::startAdvertising();
  ESP_LOGI(TAG, "Waiting for a client connection to notify...");
  #endif
  
  
  
  
 
  // Save our connection information
  //define nimBLEMidiData somewhere
  nimBLEMidiData *data = (nimBLEMidiData *) new nimBLEMidiData;
  //data->connected_ = false; //in RtMidi, this is in the parent class
  connected_ = false; 
  
  //data->client = client;
  //data->endpoint = 0;
  apiData_ = (void *) data;  //??
  //CFRelease( name );         //??
}


unsigned int MidiOutNimBLE :: getPortCount()
{
  //CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  //return MIDIGetNumberOfDestinations();
  return 1;
}


std::string MidiOutNimBLE :: getPortName(unsigned int portNumber)
{
  //must return clientName from niBLEMidiData ?
  return "fckx_seq";
}

void MidiOutNimBLE :: openPort( unsigned int portNumber)
{
  //catch a number of error states
  
  //check if a connection already exists
  //if ( apiData->connected_ ) {
 // if ( data->connected_ ) {
     if (connected_) {
    //errorString_ = "MidiOutNimBLE::openPort: a valid connection already exists!";
    ESP_LOGE(TAG, "MidiOutNimBLE::openPort: a valid connection already exists!"); 
    //error( RtMidiError::WARNING, errorString_ );  //how  to implement this error case
    return;
  }
  
#ifdef BLOCK_TEMP  
/*  
  //check if output destination exists
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  unsigned int nDest = MIDIGetNumberOfDestinations();
  if (nDest < 1) {
    errorString_ = "MidiOutNimBLE::openPort: no MIDI output destinations found!";
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }
*/
/*
  //check if the 'portNumber' argument is valid
  if ( portNumber >= nDest ) {
    std::ostringstream ost;
    ost << "MidiOutNimBLE::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }  
*/
  
 MIDIPortRef port;
  nimBLEMidiData *data = static_cast<nimBLEMidiData *> (apiData_);
  CFStringRef portNameRef = CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDIOutputPortCreate( data->client, portNameRef, &port );
  CFRelease( portNameRef );
  if ( result != noErr ) {
    MIDIClientDispose( data->client );
    errorString_ = "MidiOutNimBLE::openPort: error creating OS-X MIDI output port.";
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Get the desired output port identifier.
  MIDIEndpointRef destination = MIDIGetDestination( portNumber );
  if ( destination == 0 ) {
    MIDIPortDispose( port );
    MIDIClientDispose( data->client );
    errorString_ = "MidiOutNimBLE::openPort: error getting MIDI output destination reference.";
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }
#endif

  // Save our (api-specific) connection information.
  
  //data->port = port;  
  //data->destinationId = destination;
  connected_ = true;  
}  

void MidiOutNimBLE :: closePort( void )
{
  nimBLEMidiData *data = static_cast<nimBLEMidiData *> (apiData_);
/*
  if ( data->endpoint ) {
    MIDIEndpointDispose( data->endpoint );
    data->endpoint = 0;
  }
*/
/*
  if ( data->port ) {
    MIDIPortDispose( data->port );
    data->port = 0;
  }
*/
  connected_ = false;
}
/*
void MidiOutNimBLE :: setClientName ( const std::string& )
{

  errorString_ = "MidiOutNimBLE::setClientName: this function is not implemented for the MACOSX_CORE API!";
  //error( RtMidiError::WARNING, errorString_ );

}
*/
/*
void MidiOutNimBLE :: setPortName ( const std::string& )
{

  errorString_ = "MidiOutNimBLE::setPortName: this function is not implemented for the MACOSX_CORE API!";
  //error( RtMidiError::WARNING, errorString_ );

}
*/
/*
void MidiOutNimBLE :: openVirtualPort( const std::string &portName )
{
  nimBLEMidiData *data = static_cast<nimBLEMidiData *> (apiData_);

  if ( data->endpoint ) {
    errorString_ = "MidiOutNimBLE::openVirtualPort: a virtual output port already exists!";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::WARNING, errorString_ );
    return;
  }

  // Create a virtual MIDI output source.
  MIDIEndpointRef endpoint;
  CFStringRef portNameRef = CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDISourceCreate( data->client, portNameRef, &endpoint );
  CFRelease( portNameRef );

  if ( result != noErr ) {
    errorString_ = "MidiOutNimBLE::initialize: error creating OS-X virtual MIDI source.";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  data->endpoint = endpoint;
}
*/
void MidiOutNimBLE :: sendMessage( const std::vector<unsigned char>  *message )
//void MidiOutNimBLE :: sendMessage( const unsigned char *message, size_t size )
{
  // We use the MIDISendSysex() function to asynchronously send sysex
  // messages.  Otherwise, we use a single nimBLEMidi MIDIPacket.
  /*
  unsigned int nBytes = static_cast<unsigned int> (size);
  if ( nBytes == 0 ) {
    errorString_ = "MidiOutNimBLE::sendMessage: no data in message argument!";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::WARNING, errorString_ );
    return;
  }
  
  MIDITimeStamp timeStamp = AudioGetCurrentHostTime();
  nimBLEMidiData *data = static_cast<nimBLEMidiData *> (apiData_);
  OSStatus result;

  if ( message[0] != 0xF0 && nBytes > 3 ) {
    errorString_ = "MidiOutNimBLE::sendMessage: message format problem ... not sysex but > 3 bytes?";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::WARNING, errorString_ );
    return;
  }
 
  Byte buffer[nBytes+(sizeof( MIDIPacketList ))];
  ByteCount listSize = sizeof( buffer );
  MIDIPacketList *packetList = (MIDIPacketList*)buffer;
  MIDIPacket *packet = MIDIPacketListInit( packetList );

  ByteCount remainingBytes = nBytes;
  while ( remainingBytes && packet ) {
    ByteCount bytesForPacket = remainingBytes > 65535 ? 65535 : remainingBytes; // 65535 = maximum size of a MIDIPacket
    const Byte* dataStartPtr = (const Byte *) &message[nBytes - remainingBytes];
    packet = MIDIPacketListAdd( packetList, listSize, packet, timeStamp, bytesForPacket, dataStartPtr );
    remainingBytes -= bytesForPacket;
  }

  if ( !packet ) {
    errorString_ = "MidiOutNimBLE::sendMessage: could not allocate packet list";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Send to any destinations that may have connected to us.
  if ( data->endpoint ) {
    result = MIDIReceived( data->endpoint, packetList );
    if ( result != noErr ) {
      errorString_ = "MidiOutNimBLE::sendMessage: error sending MIDI to virtual destinations.";
      //error( RtMidiError::WARNING, errorString_ );
    }
  }
*/
/*
  // And send to an explicit destination port if we're connected.
  if ( connected_ ) {
    result = MIDISend( data->port, data->destinationId, packetList );
    if ( result != noErr ) {
      errorString_ = "MidiOutNimBLE::sendMessage: error sending MIDI message to port.";
      //error( RtMidiError::WARNING, errorString_ );
    }
  }
  */
  
}