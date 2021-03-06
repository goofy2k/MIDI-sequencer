/* implementation of class MQTTMidiIn */

/*
* FCKX december 2021
*/
#include "esp_log.h"
#include "MQTTdriver.h" 

//#include <NimBLEDevice.h>
//commenting this may have led to missing out of vTaskDelay and portTICK_PERIOD_MS
//you may have to include some freertos headers
//but which one?

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "freertos/semphr.h"
//#include "freertos/queue.h"
//#include "freertos/timers.h" //for using software timers, NOT required for the nbDelay (?)

//#include "freertos/event_groups.h"


static const char *TAG = "MQTTDRIVER";

/*  OBSOLETE
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

//bool deviceConnected = false;
//bool oldDeviceConnected = false;
//uint32_t value = 0;
#ifdef CONNECTEDTASK
 void connectedTask (void * parameter){
    for(;;) {  //loop forever
        // only act if connection status changes or if there is a stable connection
        if (deviceConnected) {
               //ESP_LOGE(TAG,"Testing NiCMidi functionality: MIDItimer MIDITickComponent, MIDIManager");
  //ESP_LOGE(TAG,"Testing NiCMidi functionality: test_midi_ports.cpp");    

//NOTE:  this is a CYCLIC TASK executed after the first connect to the BLE interface
//PUT THE INSTANTIATION PARTS IN THE MAIN_TEST_ ROUTINES OUTSIDE THIS LOOP
//OR FLAG THEM AS DONE TO PREVENT EXECUTING THEM AGAIN AND AGAIN
//THIS IS SOLVED FOR THE main_recorder EXAMPLE  
//  main_test_midiports(); //code above
  //ESP_LOGE(TAG,"Testing NiCMidi functionality: test_metronome.cpp SWITCHED OFF");
//  main_test_metronome(command); //code above
  //ESP_LOGE(TAG,"Testing NiCMidi functionality: test_component.cpp SWITCHED OFF");                                                                               
//  main_test_component(); //code above  DOESN'T EXIST 
               //A CALLBACK HERE                                                                                           
            //ESP_LOGW(TAG,"STABLE CONNECTION "); 
            /*
            //pCharacteristic->setValue((uint8_t*)&value, 4);
            midiPacket[2] = 0x90; //keyOn, channel 0
            midiPacket[4] = 127; //velocity
            pCharacteristic->setValue(midiPacket, 5);
            pCharacteristic->notify();
            //value++;
            vTaskDelay(1000/portTICK_PERIOD_MS);  // bluetooth stack will go into congestion, if too many packets are sent
            //pCharacteristic->setValue((uint8_t*)&value, 4);
            midiPacket[2] = 0x80; //keyOff, channel 0
            midiPacket[4] = 0; //velocity  
            pCharacteristic->setValue(midiPacket, 5);            
            pCharacteristic->notify();
            //value++;
            vTaskDelay(1000/portTICK_PERIOD_MS);  // bluetooth stack will go into congestion, if too many packets are sent
            */



 }
        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            ESP_LOGW(TAG,"BLE disconnected"); 
            ESP_LOGW(TAG,"Do required stuff, depending on the needs for this lost connection");
            vTaskDelay(500/portTICK_PERIOD_MS); // give the bluetooth stack the chance to get things ready
  //vTaskDelay(5000); // give the bluetooth stack the chance to get things ready
  //portTICK_PERIOD_MS unknown in this scpoe !?!?!?
            ESP_LOGW(TAG,"Start advertising again"); 
            //DO THIS EXERCISE INSIDE THE MidiOutNimBLE CLASS            
            //pServer->startAdvertising();        // restart advertising
            //printf("start advertising\n");
            oldDeviceConnected = deviceConnected;
        }
        // connection established
        if (deviceConnected && !oldDeviceConnected) {
            ESP_LOGW(TAG,"BLE connected, do required stuff, depending on the needs for this connection");
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
        }
        
        vTaskDelay(10/portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer
    }
    
    vTaskDelete(NULL);
}
#endif // CONNECTEDTASK   

//MIDI data packet, taken from Brian Duhan arduino-esp32-BLE-MIDI / BLE_MIDI.ino

/*  //uint8_t midiPacket already defined in nimBLEdriver.cpp
    //PROBABLY WISE TO CREATE/RE-USE A STRUCTURE/CLASS WHERE MQTTdriver and niBLEdriver inherit this (AND MORE) from
uint8_t midiPacket[] = {
    0x80, //header
    0x80, //timestamp, not implemented
    0x00, //status
    0x3c, //== 60 == middle c
    0x00  //velocity
};
*/

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#ifdef BLOCKED
/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */  
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      static const char *TAG = "NIMBLE_ONCONNECT";
      ESP_LOGW(TAG, "-----");  
      deviceConnected = true;
      BLEDevice::startAdvertising(); //keep advertising to find more connections
    };

    void onDisconnect(BLEServer* pServer) {
      static const char *TAG = "NIMBLE_ONDISCONNECT";
      ESP_LOGW(TAG, "-----");   
      deviceConnected = false;
    }
/***************** New - Security handled here ********************
****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest(){
    static const char *TAG = "NIMBLE_ONPASSKEYREQUEST";
    ESP_LOGW(TAG, "-----");   
    printf("Server PassKeyRequest\n");
    return 123456; 
  }

  bool onConfirmPIN(uint32_t pass_key){
    static const char *TAG = "NIMBLE_ONCONFIRMPIN";
    ESP_LOGW(TAG, "-----");  
    printf("The passkey YES/NO number: %d\n", pass_key);
    return true; 
  }

  void onAuthenticationComplete(ble_gap_conn_desc desc){
    static const char *TAG = "NIMBLE_ONAUTHENTICATIONCOMPLETE";
    ESP_LOGW(TAG, "-----");  
    printf("Starting BLE work!\n");
  }
  };
#endif //BLOCKED  
  
/*******************************************************************/

/*
 //MOVE STRUCT definition to header file
struct NimBLEMidiOutData {
  NimBLEServer* client;//this "client" is actually a nimBLE server
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
*/

 //see RtMidi.cpp for detailed implementation
 //class MidiInAPI for hardware independent part
//othe classes for hardware implementation 
 
 MQTTMidiIn :: MQTTMidiIn ()
//MQTTMidiIn :: MQTTMidiIn (const std::string &clientName)
{   
    //MQTTMidiIn::initialize("fckx_seq2"); 
    //MQTTMidiIn::initialize(clientName);    
}

MQTTMidiIn :: ~MQTTMidiIn ()
{
    //close a connection if it exists
    MQTTMidiIn::closePort();
    
    //Cleanup
    //what does this comprise for nimBLEDevice ?
    
}


void MQTTMidiIn :: openPort( unsigned int portNumber) {
  ESP_LOGW(TAG, "open MQTTMidiIn port"); 
}

#ifdef BLOCKMIDIINNIMBLE
//void MQTTMidiIn :: openPort( )
void MidiInNimBLE :: openPort( unsigned int portNumber)
{
   
   // Set up our client and give a sign of life
  ESP_LOGW(TAG, "open MQTTMidiIn port"); 
  
  NimBLEServer* pServer = NULL;  //must be made accessible for the outside world later
  NimBLECharacteristic* pCharacteristic = NULL; 
  //Create the BLE Device 
  //This is also for Midi input, so should ideally be in a super class
  NimBLEDevice::init("fckx_seq"); //later use clientName to pass the name
                                  //and include the unique device ID in it
                                  //take care of max length of this string!
  
  ESP_LOGW(TAG, "NimBLEDevice created"); 
#define TEMP_BLOCK1 1  
 #ifdef TEMP_BLOCK1 
  pServer = NimBLEDevice::createServer();
     ESP_LOGW(TAG, "BLE server created"); 
  pServer->setCallbacks(new MyServerCallbacks()); //NOTE: this uses the DEFAULT callbacks in the library
   ESP_LOGW(TAG, "BLE server callbacks created"); 
   
  // Create the BLE Service
  NimBLEService *pService = pServer->createService(SERVICE_UUID);
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
                      NIMBLE_PROPERTY::NOTIFY //|
                    //  NIMBLE_PROPERTY::INDICATE
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
  /*
  //MOVE THIS TO openPort

  // Start advertising
  ESP_LOGI(TAG, "Start advertising");
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false); 
*/
  #endif
  
  
    // Save our connection information
    
    connectionData.pServer = pServer;
      connectionData.pService = pService;
        connectionData.pCharacteristic = pCharacteristic;
  
 /*  
  // Save our connection information
  //define NimBLEMidiOutData somewhere
  NimBLEMidiOutData *data = (NimBLEMidiOutData *) new NimBLEMidiOutData;
  //data->connected_ = false; //in RtMidi, this is in the parent class
  connected_ = false; 
  
  //data->client = client;
  //data->endpoint = 0;
  apiData_ = (void *) data;  //??
  //CFRelease( name );         //??
  */ 
    
    
    
    
    
    
    
    
    
    
    
    //catch a number of error states    
    printf("OPENPORT ENTERED");
    ESP_LOGW(TAG, "MQTTMidiIn :: openPort entered "); 




    //start background task HERE????
    // depends on kind of task
    xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);
    
      
  //MOVE THIS TO openPort
  

  // Start advertising
  ESP_LOGI(TAG, "Prepare advertising");
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false); 
  
  connectionData.pAdvertising = pAdvertising;
    
      // Start advertising
  ESP_LOGI(TAG, "Start advertising");

    NimBLEDevice::startAdvertising();
    ESP_LOGI(TAG, "OPENPORT Waiting for a client connection to notify...");

    //check if a connection already exists
    //if ( apiData->connected_ ) {
    // if ( data->connected_ ) {
        
     //DO THIS BEFORE PREP OF ADVERTISING?   
     if (connected_) {
    //errorString_ = "MQTTMidiIn::openPort: a valid connection already exists!";
    ESP_LOGE(TAG, "MQTTMidiIn::openPort: a valid connection already exists!"); 
    //error( RtMidiError::WARNING, errorString_ );  //how  to implement this error case
    return;
  }
  
#ifdef BLOCK_TEMP  
/*  
  //check if output destination exists
  CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  unsigned int nDest = MIDIGetNumberOfDestinations();
  if (nDest < 1) {
    errorString_ = "MQTTMidiIn::openPort: no MIDI output destinations found!";
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::NO_DEVICES_FOUND, errorString_ );
    return;
  }
*/
/*
  //check if the 'portNumber' argument is valid
  if ( portNumber >= nDest ) {
    std::ostringstream ost;
    ost << "MQTTMidiIn::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
    errorString_ = ost.str();
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::INVALID_PARAMETER, errorString_ );
    return;
  }  
*/
  
 MIDIPortRef port;
  NimBLEMidiOutData *data = static_cast<NimBLEMidiOutData *> (apiData_);
  CFStringRef portNameRef = CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDIOutputPortCreate( data->client, portNameRef, &port );
  CFRelease( portNameRef );
  if ( result != noErr ) {
    MIDIClientDispose( data->client );
    errorString_ = "MQTTMidiIn::openPort: error creating OS-X MIDI output port.";
    ESP_LOGE(TAG, "%s" , errorString);
    //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Get the desired output port identifier.
  MIDIEndpointRef destination = MIDIGetDestination( portNumber );
  if ( destination == 0 ) {
    MIDIPortDispose( port );
    MIDIClientDispose( data->client );
    errorString_ = "MQTTMidiIn::openPort: error getting MIDI output destination reference.";
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
#endif // BLOCKMIDIINNIMBLE


void MQTTMidiIn :: closePort( void ) {

   //why instantiate here, on closing ???????
  //how is this done in RtMidi   
  //there it is used in every procedure that accessess MidiData
  //it is probably a way to have a temp (local) access to the buffer etc
  //so you have to understand the code
  //....
  
  /*
  in RtMidi there is some jiggling with pointers in the initialize parts
  e.g.
  CoreMidiData *data = (CoreMidiData *) new CoreMidiData;
  data->client = client;
  data->endpoint = 0;
  apiData_ = (void *) data;
  inputData_.apiData = (void *) data;
  
  THIS MAY HAVE TO DO WITH THE FOLLOWIN ERROR
  */
  
  
  
  
   
   MQTTMidiInData *data = static_cast< MQTTMidiInData *> (inputData_); 
   //error: invalid static_cast from type 'MQTTMidiIn::MQTTMidiInData' to type 'MQTTMidiIn::MQTTMidiInData*'
   //error disappears when changing MQTTMidiInData  inputData_; in MQTTdriver.h into  MQTTMidiInData * inputData_; 
   //but leads to:MQTTdriver.cpp:232:19: error: request for member 'usingCallback' in '((MQTTMidiIn*)this)->MQTTMidiIn::inputData_', which is of pointer type 'MQTTMidiIn::MQTTMidiInData*' (maybe you meant to use '->' ?)

   
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
//unsubscribe MQTT??
//update MQTTMidiInData

 // connected_ = false;
}


bool MQTTMidiIn :: isPortOpen( ) {
    
  return connected_;  
}



unsigned int MQTTMidiIn :: getPortCount()
{
  //CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
  //return MIDIGetNumberOfDestinations();
  return 1;
}


std::string MQTTMidiIn :: getPortName(unsigned int portNumber)
{
  //must return clientName from niBLEMidiData ?
  return "fckx_seq";
}


double MQTTMidiIn :: getMessage( std::vector<unsigned char> *message )
{
  message->clear();
  //if ( inputData_.usingCallback )  //FCKX!
  if ( inputData_->usingCallback ) 
  {
   // errorString_ = "RtMidiIn::getNextMessage: a user callback is currently set for this port.";
   // error( RtMidiError::WARNING, errorString_ );
    ESP_LOGE(TAG, "RtMidiIn::getNextMessage: a user callback is currently set for this port."); //error( RtMidiError::DRIVER_ERROR, errorString_ );

    return 0.0;
  }

  double timeStamp;
    if ( !inputData_->queue.pop( message, &timeStamp ) )
 // if ( !inputData_.queue.pop( message, &timeStamp ) ) //FCKX!
    return 0.0;

  return timeStamp;
}



void MQTTMidiIn :: setCallback( MQTTMidiIn::MQTTMidiCallback callback, void *userData )
{
    if ( inputData_->usingCallback ) {  //FCKX!
  //if ( inputData_.usingCallback ) {
    //errorString_ = "MidiInApi::setCallback: a callback function is already set!";
 
    ESP_LOGE(TAG,"MidiInApi::setCallback: a callback function is already set!"); 
    //error( RtMidiError::WARNING, errorString_ );
    return;
  }

  if ( !callback ) {
    //errorString_ = "RtMidiIn::setCallback: callback function value is invalid!";
    //error( RtMidiError::WARNING, errorString_ );
    ESP_LOGE(TAG,"RtMidiIn::setCallback: callback function value is invalid!");

    return;
  }
/*
  inputData_.userCallback = callback;
  inputData_.userData = userData;
  inputData_.usingCallback = true;
*/ //FCKX! 

  inputData_->userCallback = callback;
  inputData_->userData = userData;
  inputData_->usingCallback = true; 
}

void MQTTMidiIn :: ignoreTypes( bool midiSysex, bool midiTime, bool midiSense )
{

  /* //FCKX!
    inputData_.ignoreFlags = 0;
  if ( midiSysex ) inputData_.ignoreFlags = 0x01;
  if ( midiTime ) inputData_.ignoreFlags |= 0x02;
  if ( midiSense ) inputData_.ignoreFlags |= 0x04;
  */
  inputData_->ignoreFlags = 0;
  if ( midiSysex ) inputData_->ignoreFlags = 0x01;
  if ( midiTime ) inputData_->ignoreFlags |= 0x02;
  if ( midiSense ) inputData_->ignoreFlags |= 0x04;
  
}



  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is best
    to set the error callback function before opening a port.
  */
//  virtual void setErrorCallback( MQTTMidiCallback errorCallback = NULL, void *userData = 0 );



//initialize is only present in the classes nearest to the hardware 
/*
void MQTTMidiIn :: initialize ( const std::string& clientName)
{
  
}
*/

/*
void MQTTMidiIn :: setClientName ( const std::string& )
{

  errorString_ = "MQTTMidiIn::setClientName: this function is not implemented for the MACOSX_CORE API!";
  //error( RtMidiError::WARNING, errorString_ );

}
*/
/*
void MQTTMidiIn :: setPortName ( const std::string& )
{

  errorString_ = "MQTTMidiIn::setPortName: this function is not implemented for the MACOSX_CORE API!";
  //error( RtMidiError::WARNING, errorString_ );

}
*/
/*
void MQTTMidiIn :: openVirtualPort( const std::string &portName )
{
  NimBLEMidiOutData *data = static_cast<NimBLEMidiOutData *> (apiData_);

  if ( data->endpoint ) {
    errorString_ = "MQTTMidiIn::openVirtualPort: a virtual output port already exists!";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::WARNING, errorString_ );
    return;
  }

  // Create a virtual MIDI output source.
  MIDIEndpointRef endpoint;
  CFStringRef portNameRef = CFStringCreateWithCString( NULL, portName.c_str(), kCFStringEncodingASCII );
  OSStatus result = MIDISourceCreate( data->client, portNameRef, &endpoint );
  CFRelease( portNameRef );

  if ( result != noErr ) {
    errorString_ = "MQTTMidiIn::initialize: error creating OS-X virtual MIDI source.";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Save our api-specific connection information.
  data->endpoint = endpoint;
}
*/


#ifdef BLOCK2
double MQTTMidiIn :: getMessage( const std::vector<unsigned char>  *message )
//void MQTTMidiIn :: sendMessage( const unsigned char *message, size_t size )
{
    ESP_LOGW(TAG, "sendMessage  TO BE IMPLEMENTED"); 
    /*
    ESP_LOGW(TAG, "message.size %d", message->size());
    for (unsigned i=0; i<message->size(); i++)
    ESP_LOGW(TAG, "message.size %d", message->at(i));
*/
    //raw version copied from main::sendToMIDIOut
        //prepare for sending to output
    //convert MIDIMessage to midiPacket
    static const char *TAG = "sendToMIDIOut";  
    //unsigned long int mididata;
    uint8_t midiPacket[8];
    unsigned char header;
    unsigned char timestamp_low;
    unsigned char timestamp_high;
    unsigned char midi_status;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    
    ESP_LOGI(TAG,"Prepare for sending data to output");

    
    //check raw byte values, befor implementing playing to BLE notify (output)
    //write a notify procedure that accepts MIDIMessage as input

       //check raw byte values, befor implementing playing to BLE notify (output)
    //write a notify procedure that accepts MIDIMessage as input
    
 //   printMsgBytes(msg1); //for debugging/devel
    //convert to mididata = Process (?)  you also may want to filter some messages
    /***************************************************************************************************************************** 
    * Midi data over BLE are sent exchanged according to the Specification for MIDI over Bluetooth Low Energy (BLE-MIDI, see docs)
    * Currently only 5 byte packets with single messages are supported
    *
    * Currently NOT supported are:
    *   - running status messages
    *   - multi-message packets
    *
    * Also, only Note on and Note Off messages are supported by the receiving sound board
    * MIDI_BLE supports a 13-bit timestamp, but these are not (yet) interpreted by the receiver. This application intends to send messages that should be played
    * immediately ("real-time") by the sound board
    * see: https://h2zero.github.io/esp-nimble-cpp/index.html
    * see: https://h2zero.github.io/esp-nimble-cpp/md_docs__usage_tips.html
    *
    ******************************************************************************************************************************/


   for (unsigned i=0; i<message->size(); i++) {
    ESP_LOGW(TAG, "message.size %d", message->at(i));
    midiPacket[i] = message->at(i);
   };

     /*
    timestamp_high = 0;
    timestamp_low = 0;
    

    midi_status = msg1.GetStatus();
    byte1 = msg1.GetByte1();
    byte2 = msg1.GetByte2();
    byte3 = msg1.GetByte3();
    */
    /*
    header = 0b11000000 + timestamp_high;
    midiPacket[0] = header;
    midiPacket[1] = 0b1000000 + timestamp_low;
    midiPacket[2] = midi_status; //byte1? 
    midiPacket[3] = byte1;  //byte2?
    midiPacket[4] = byte2;  //byte3?
  */
  /*
    //test data
    midiPacket[0] = 0x00;
    midiPacket[1] = 0x01;
    midiPacket[2] = 0x02;
    midiPacket[3] = 0x03;
    midiPacket[4] = 0x04;  
  */
    //<code> here
    
    
    
    //send to "hardware" interface   //WORDT DUS: receive from
    //<code here>
    
    

    //FCKX FCKX FCKX
    //connectionData.pCharacteristic->setValue(midiPacket, message->size());
    //connectionData.pCharacteristic->notify(); 
    
    //ble_notify_midi(pCharacteristic, mididata); this was suitable for midi thru


//METHOD2....
     
  // We use the MIDISendSysex() function to asynchronously send sysex
  // messages.  Otherwise, we use a single nimBLEMidi MIDIPacket.
  /*
  unsigned int nBytes = static_cast<unsigned int> (size);
  if ( nBytes == 0 ) {
    errorString_ = "MQTTMidiIn::sendMessage: no data in message argument!";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::WARNING, errorString_ );
    return;
  }
  
  MIDITimeStamp timeStamp = AudioGetCurrentHostTime();
  NimBLEMidiOutData *data = static_cast<NimBLEMidiOutData *> (apiData_);
  OSStatus result;

  if ( message[0] != 0xF0 && nBytes > 3 ) {
    errorString_ = "MQTTMidiIn::sendMessage: message format problem ... not sysex but > 3 bytes?";
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
    errorString_ = "MQTTMidiIn::sendMessage: could not allocate packet list";
    ESP_LOGE(TAG, "%s" , errorString); //error( RtMidiError::DRIVER_ERROR, errorString_ );
    return;
  }

  // Send to any destinations that may have connected to us.
  if ( data->endpoint ) {
    result = MIDIReceived( data->endpoint, packetList );
    if ( result != noErr ) {
      errorString_ = "MQTTMidiIn::sendMessage: error sending MIDI to virtual destinations.";
      //error( RtMidiError::WARNING, errorString_ );
    }
  }
*/
/*
  // And send to an explicit destination port if we're connected.
  if ( connected_ ) {
    result = MIDISend( data->port, data->destinationId, packetList );
    if ( result != noErr ) {
      errorString_ = "MQTTMidiIn::sendMessage: error sending MIDI message to port.";
      //error( RtMidiError::WARNING, errorString_ );
    }
  }
  */
  
}
#endif