//#define TEST_ADVANCEDSEQUENCER_NOINPUT //RUNS OK "succesfully" looping twinkle twinkle
//#define TEST_RECORDING_NO_INPUT               //in development/
#define TEST_RECORDER  //with NR GUI input
//#define TEST_THRU                         //input and output   RUNS OK


#define DRUMPRESET 0x40 //64

//CONSIDER TO RENAME BLEDevice etc to NimBLEDevice etc. But test carefully when you have running code!
//remove references to the old jdks lib
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sstream> //for ost /stringbuf
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h" //for using software timers, NOT required for the nbDelay (?)

#include "freertos/event_groups.h"

#include "esp_spi_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

//#include <map>  //seq_command dispatcher

esp_mqtt_client_handle_t  mqtt_client;


#include "nimBLEdriver.h" //make driver globally accessible by including this header file

//for test_recorder
#include "../include/advancedsequencer.h"
#include "../include/recorder.h"
#include "../include/filewritemultitrack.h"
#include "../examples/functions.h"                  // helper functions for input parsing and output and Dump

//#define DEBUGON
#ifdef DEBUGON
//determine hardware concurrency, for debugging only
// https://en.cppreference.com/w/cpp/thread/thread/hardware_concurrency
#include <iostream>
#include <thread>
#endif

/* NiCMidi examples             MIDI     MIDI      COMMAND       FILE     FILE            WINDOWS
                                OUTPUT   INPUT     CONSOLE       READ     WRITE    EDIT    GUI
test_advancedsequencer.cpp       X        -           X            X        -       -
test_advancedsequencer_noinput   X        -                        -        -       -
test_component.cpp               X        -           X            -        -       -
test_metronome.cpp               X        -           X            -        -       -
test_midiports.cpp               -        -           X            -        -       -
test_recorder.cpp                X        X           X            X        X       -
test_sequencer.cpp               X        -           X            X        -       -
test_stepsequencer.cpp           -        -           X            X        X       X
test_thru.cpp                    X        X           X            -        -       -
test_writefile.cpp               X        -           -            -        X       -
test_win32_player.cpp                                              X                        X 
*/


//the following (adapted) examples depend on nimBLEdriver bluetooth output by FCKX
//use only one at a time as instantiation of output ports may interfere (solve this in the future for flxibility)


/*
https://www.esp32.com/viewtopic.php?t=18054
Though highly unlikely but as a starting debug point can you please increase NIMBLE_STACK size from 512 to say 4096 (`CONFIG_BT_NIMBLE_TASK_STACK_SIZE`)?
I suspect the error might be coming from `ble_buf_alloc` as well. Can you please try reducing the buffer counts configured with `CONFIG_BT_NIMBLE_HCI_EVT_HI_BUF_COUNT = 8` and `CONFIG_BT_NIMBLE_ACL_BUF_COUNT = 6`.
*/


#include "secrets.h"

//Credentials form secrets.h
#define EXAMPLE_ESP_WIFI_SSID      SECRET_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      SECRET_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  SECRET_ESP_MAXIMUM_RETRY


#include "msg.h"
#include <iostream>
#include "track.h"
#include "dump_tracks.h"
#include "sysex.h"
#include "../include/tick.h"       //check if you can harmonize these paths FCKX
#include "../include/manager.h"

extern "C" {void app_main(void);}

NimBLECharacteristic* pGUICharacteristic;
#define GUI_CHARUUID "109d98fa-9f22-11ec-b909-0242ac120002"

#ifdef NIMBLE_IN_MAIN
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  NIMBLE_IN_MAIN");
//test using "NIM" before "BLE"
BLEServer* pServer = NULL; //FCKX phase out //compile conditionally
BLECharacteristic* pCharacteristic = NULL;  //FCKX phase out
#endif

bool dumpflag;

static const char *TAG = "FCKX_SEQ";


void nbDelay(int delayTicks) {      //NON-BLOCKING DELAY HELPER  
        TickType_t startTick = xTaskGetTickCount();
        while( (xTaskGetTickCount()-startTick) < pdMS_TO_TICKS(delayTicks)){
          //…
        } 
    };


/* WIFI functionality, taken from ESP-IDF example: xxxxxxxxxxxx
*
*  modifications: xxxxxxxxxxxx
*/

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static int s_retry_num = 0;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data){
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    esp_event_loop_delete_default(); //FCKX
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    
    //https://esp32.com/viewtopic.php?t=1317
    //You can init structs in C++ but all elements of the struct has to be included in the initialization in the order they are declared as you can not reference properties by name. 
    //Luckily, in C++, the struct can be initialized with {} and each property assigned separately.
    
    wifi_config_t wifi_config = {};  
    strcpy((char*)wifi_config.sta.ssid, EXAMPLE_ESP_WIFI_SSID );
    strcpy((char*)wifi_config.sta.password, EXAMPLE_ESP_WIFI_PASS);
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by wifi_event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
                ESP_LOGI(TAG, "connected to ap SSID:%s password: XXXXXXXXXXXX",
                 EXAMPLE_ESP_WIFI_SSID);
     /*   ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS); */
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    /* The event will not be processed after unregister */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(s_wifi_event_group);
}
/*
* END of WIFI functionality
*/


#ifdef NIMBLE_IN_MAIN  //phase out
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  NIMBLE_IN_MAIN");
 void ble_notify_midi(BLECharacteristic* pCharacteristic, unsigned long int mididata){
    static const char *TAG = "ble_notify_midi";
    //MIDI data packet, taken from Brian Duhan arduino-esp32-BLE-MIDI / BLE_MIDI.ino

    //you address 5 bytes in mididata, but unsigned long int has 4???
    //NO!  mididata is 4 bytes,   midiPacket is 5 bytes
    //have a good look!
    //do you want to send timestamp?
    //you probably want to send a midiPacket according to the BLE MIDI standard ! See docs 

    uint8_t midiPacket[] = {                           
        0x80, //header
        0x80, //timestamp, not implemented
        0x00, //status
        0x3c, //== 60 == middle c
        0x00  //velocity
    };//

    ESP_LOGW(TAG,"CREATE and notify midiPacket");    //wrap this into a routine using a MIDITimedBigMessage as input (not necessary for incoming
    midiPacket[4] =  mididata & 0x00000000ff;
    midiPacket[3] = (mididata & 0x000000ff00)>>8;
    midiPacket[2] = (mididata & 0x0000ff0000)>>16;
    midiPacket[1] = (mididata & 0x00ff000000)>>24;
    midiPacket[0] = (mididata & 0xff00000000)>>32;
    //midipacket[1] = ;
    //midipacket[0] = ;
   ESP_LOGI(TAG,"NUMERICAL VALUE:%lu ", mididata);
   ESP_LOGI(TAG,"HEADER: %u (0x%X)", midiPacket[0], midiPacket[0]);
   ESP_LOGI(TAG,"TIMESTAMP: %u (0x%X)", midiPacket[1], midiPacket[1]);
    ESP_LOGI(TAG,"STATUS: %u (0x%X)", midiPacket[2], midiPacket[2]);
    ESP_LOGI(TAG,"DATA1: %u (0x%X)", midiPacket[3], midiPacket[3]);
    ESP_LOGI(TAG,"DATA2: %u (0x%X)", midiPacket[4], midiPacket[4]) ; //integers are 32 bits!!!!             
 
         //   midiPacket[2] = 0x90; //keyOn, channel 0
         //   midiPacket[4] = 127; //velocity    
    
            pCharacteristic->setValue(midiPacket, 5);
            pCharacteristic->notify();   
}

#endif


#define CUSTOM_OST
//code for writing to custom output port, useful for re-directing UI/GUI output to e.g. MQTT
//http://videocortex.io/2017/custom-stream-buffers/  //not suited for C++11
//https://codereview.stackexchange.com/questions/185490/custom-ostream-for-a-println-like-function
//https://coderedirect.com/questions/196423/a-custom-ostream //
//https://www.cprogramming.com/tutorial/c++-iostreams.html
#ifdef CUSTOM_OST



#endif //CUSTOM_OST



void printMsgBytes(MIDIMessage msg1){   //add an identifier text
    static const char *TAG = "printMsgBytes";  
    char length =  msg1.GetLength();
    //long unsigned int timestamp = msg1.GetTime();  //MIDIMessage has no GetTime() member,  MIDITimedMessage does
    unsigned char status = msg1.GetStatus();
    unsigned char channel = msg1.GetChannel();
    unsigned char type = msg1.GetType();
    unsigned char metatype = msg1.GetMetaType(); 
    unsigned char byte1 = msg1.GetByte1();
    unsigned char byte2 = msg1.GetByte2();
    unsigned char byte3 = msg1.GetByte3();
    ESP_LOGI(TAG,"msg1 length %d", length);
    //ESP_LOGI(TAG,"msg1 length %lu", timestamp);
    ESP_LOGI(TAG,"msg1 status %u (0x%x)", status,status);
    ESP_LOGI(TAG,"msg1 channel %u (0x%x)", channel, channel);
    ESP_LOGI(TAG,"msg1 type %u (0x%x)", type, type);
    ESP_LOGI(TAG,"msg1 metatype %u (0x%x)", metatype, metatype);    
    ESP_LOGI(TAG,"msg1 byte1 %u (0x%x)", byte1, byte1);
    ESP_LOGI(TAG,"msg1 byte2 %u (0x%x)", byte2, byte2);
    ESP_LOGI(TAG,"msg1 byte3 %u (0x%x)", byte3, byte3);     
};

#ifdef NIMBLE_IN_MAIN  //phase out
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  NIMBLE_IN_MAIN");
void sendToMIDIOut (MIDIMessage msg1) {  
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
    
    printMsgBytes(msg1); //for debugging/devel
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
    timestamp_high = 0;
    timestamp_low = 0;
    midi_status = msg1.GetStatus();
    byte1 = msg1.GetByte1();
    byte2 = msg1.GetByte2();
    byte3 = msg1.GetByte3();
    
    header = 0b11000000 + timestamp_high;
    midiPacket[0] = header;
    midiPacket[1] = 0b1000000 + timestamp_low;
    midiPacket[2] = midi_status; //byte1? 
    midiPacket[3] = byte1;  //byte2?
    midiPacket[4] = byte2;  //byte3?
    
    //<code> here
    //send to "hardware" interface
    //<code here>
    pCharacteristic->setValue(midiPacket, 5);
    pCharacteristic->notify(); 
    //ble_notify_midi(pCharacteristic, mididata); this was suitable for midi thru
    
};


void fckx_single_midi_command(unsigned long int mididata){  //uses propagateMidi
static const char *TAG = "execute_single_midi_command";
      //unsigned long eventTimestamp = generate_Timestamp(xTaskGetTickCount());
    //unsigned long event_Timestamp =  generate_Timestamp( xTaskGetTickCount() ) ;
    //jdksmidi::MIDITimedBigMessage inMessage = generate_MIDITimedBigMessage( mididata, event_Timestamp);
  ble_notify_midi(pCharacteristic, mididata);


  
  /*  
    if (!(seq_mode & play_incoming_mode) == 0) {    
        //and play
     propagate_Midi_Event(aDSP, mididata);
     

    };
    
    
    if (!(seq_mode & record_mode) == 0) {
        //record
        ESP_LOGI(TAG,"RECORD MIDI EVENT (start of code)");
        

            if (myQueue.CanPut()){
            myQueue.Put(inMessage); //store inMessage in queue and move next_in index up by one position
                                    //messages are in the queue in order of the moment that they are added. Especially for e.g. looping applications
                                    //the messages are NOT in order of the time that the events need to be fired
                                    
                                    //add an option to insert a message in the right position. This is at the cost of effort at insertion, but prevents the need for sorting later
                                    //this may be a similar effort, and use similar or the same basic handling....
                                    
                                    //for firing of events, timers based on the actual moment of activation can be used
                                    //to prevent long timer / output queues the main message queue must be sorted now and then. THis is also done in the jdksmidi lib
                                    //by the track subclass 
            
            } else { ESP_LOGE(TAG,"cannot Put msg to queue");} 
            //display length of queue or get some other indication of it's contents....
            //ESP_LOGE(TAG,"myQueue.next_in %d", myQueue.next_in);
            
          ESP_LOGI(TAG,"RECORD MIDI EVENT (end of code)");  

        };
  
*/

  
 };

#endif

 

void printMIDI_Input(esp_mqtt_event_handle_t event){  
    //display incoming data 
    static const char *TAG = "printMIDI_Input"; 
    ESP_LOGD(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
    ESP_LOGD(TAG,"DATA:%.*s ", event->data_len, event->data);
    ESP_LOGD(TAG,"DATA0 %d", event->data[0]);
    ESP_LOGD(TAG,"DATA1 %d", event->data[1]); 
    ESP_LOGD(TAG,"DATA2 %d", event->data[2]); 
    ESP_LOGD(TAG,"DATA3 %d", event->data[3]); 
    ESP_LOGD(TAG,"DATA4 %d", event->data[4]);
};




class MidiCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic* pCharacteristic) {
        static const char *TAG = "ONREAD [MIDI NimBLE]";
        // Do something before the read completes.
        std::cout << "Characteristic onRead FCKX" << std::endl;
        //pCharacteristic->setValue(os.str());
    }
     
    void onWrite(BLECharacteristic* pCharacteristic) {
        static const char *TAG = "ONWRITE [MIDI NimBLE]";
        // Do something because a new value was written.
        //std::cout << "pCharacteristic onWrite handler" << std::endl;
        std::string value = pCharacteristic->getValue();
        ESP_LOGD(TAG,"pCharacteristic->getValue() %s", value.c_str());
        //std::cout << "Characteristic value: "<< value << std::endl;
        //std::cout << "value.find('/'): "<< value.find('/') << std::endl;
        
        
        
        //decode value and send to control
        //start with just forwarding it
        //need to convert  value string to const * char for use in mqtt publish


        if (value.find('/') == 0) {
            //if first character is /, it contains a controller path, that can serve as an MQTT topic 
            //decode and forward over MQTT
            //separate topic and value on space and decode it
            int mypos = value.find(' ');           
            std::string myaddress = value.substr(0,mypos); 
            std::string myvalue = value.substr(mypos,value.length());            
            //forward to NODERED over MQTT
            esp_mqtt_client_publish(mqtt_client, myaddress.c_str(), myvalue.c_str(), 0, 1, 0); 
        }
        else {  //POSSIBLY OBSOLETE.....
            //decode as GUI message and process further in Nodered
            ESP_LOGD(TAG,"GUI event %s", value.c_str());
            //forward to NODERED over MQTT with a suitable topic
            esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI/", value.c_str(), 0, 1, 0); 
        }
    }
};


void setMidiCharacteristicCallBacks(void){    
    // now define CharacteristicCallBacks
    bool myresult = false;
    MidiOutNimBLE::NimBLEMidiOutData connectionData;
    while ( myresult == false){
   // while (  ! MIDIManager::GetOutDriver(0)->IsPortOpen() ) {
        std::cout << "WAIT FOR PEER1" << std::endl;
        //expose NimBLE connectionData 
         connectionData = MIDIManager::GetOutDriver(0)->Get_connectionData();  
         myresult = (connectionData.all_pCharacteristics[0] != NULL);
         
         std::cout << "WAIT FOR PEER on MIDI out" << std::endl;
        
    }
    std::cout << "set Midi Characteristic callbacks" << std::endl;
     connectionData.all_pCharacteristics[0]->setCallbacks(new MidiCharacteristicCallbacks()); 

    };  //setMidiCharacteristicCallBacks


class GUICharacteristicCallbacks: public BLECharacteristicCallbacks { 

    void onRead(BLECharacteristic* pCharacteristic) {
        static const char *TAG = "ONREAD [GUI NimBLE]";
        // Do something before the read completes.
        std::cout << "Characteristic onRead FCKX" << std::endl;
        //pCharacteristic->setValue(os.str());
    }
     
    void onWrite(BLECharacteristic* pCharacteristic) {
        static const char *TAG = "ONWRITE [GUI NimBLE]";
        // Do something because a new value was written.
        //std::cout << "pCharacteristic onWrite handler" << std::endl;
        std::string value = pCharacteristic->getValue();
        ESP_LOGD(TAG,"pCharacteristic->getValue() %s", value.c_str());
        std::cout << "Characteristic value: "<< value << std::endl;
        //decode value and send to control
        //start with just forwarding it
        //need to convert  value string to const * char for use in mqtt publish
        //separate topic and value on space

        int mypos = value.find(' ');
        std::string myaddress = value.substr(0,mypos); 
        std::string myvalue = value.substr(mypos,value.length());
        //forward to NODERED over MQTT
     //   esp_mqtt_client_publish(mqtt_client, myaddress.c_str(), myvalue.c_str(), 0, 1, 0);   
    }

   
};


void setGuiCharacteristicCallBacks(void){    
    // now define CharacteristicCallBacks
    bool myresult = false;
    MidiOutNimBLE::NimBLEMidiOutData connectionData;
    while ( myresult == false){
   // while (  ! MIDIManager::GetOutDriver(0)->IsPortOpen() ) {
        std::cout << "WAIT FOR PEER1" << std::endl;
        //expose NimBLE connectionData 
         connectionData = MIDIManager::GetOutDriver(0)->Get_connectionData();  
         myresult = (connectionData.all_pCharacteristics[1] != NULL);
         
         std::cout << "WAIT FOR PEER on MIDI out" << std::endl;
        
    }
    std::cout << "set Gui Characteristic callbacks" << std::endl;
    connectionData.all_pCharacteristics[1]->setCallbacks(new GUICharacteristicCallbacks()); 

    };  //setMidiCharacteristicCallBacks





//setting callbacks for the old local version
void setGUICharacteristicCallBacks2(void){
    // now define CharacteristicCallBacks
     std::cout << "set GUI Characteristic callbacks" << std::endl;
     pGUICharacteristic->setCallbacks(new GUICharacteristicCallbacks());    
}; // setGUICharacteristicCallBacks

//obsolete here, it is in nimBLEdriver
void createGUICharacteristic( void) {
    static const char *TAG = "CREATEGUICHARACTERISTIC";
    //MidiOutNimBLE::NimBLEMidiOutData connectionData;
    MidiOutNimBLE::NimBLEMidiOutData connectionData = MIDIManager::GetOutDriver(0)->Get_connectionData(); 
    NimBLEService *pService = connectionData.pService;

    pGUICharacteristic = pService->createCharacteristic(
                      GUI_CHARUUID,
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
  ESP_LOGW(TAG, "BLE server GUI characteristic created");

    
   //use:
  //static BLEUUID gui_charUUID("109d98fa-9f22-11ec-b909-0242ac120002"); //GUI messages  
    
    
    
}; //createGUICharacteristic  





#ifdef TEST_RECORDER 


//https://stackoverflow.com/questions/26455952/error-c-map-does-not-name-a-type

enum class Seq_command {
    A, QUIT, B, INVALID    
};

std::map< std::string, Seq_command > known_seq_commands = 
{
    
    { "A", Seq_command::A },
    { "quit", Seq_command::QUIT },
    { "B", Seq_command::B },
};

//auto parseCommand( std::string const& command ) -> seq_command
int parseCommand( std::string const& command )
    //-> seq_command
{
     static const char *TAG = "PARSE_COMMAND";
     ESP_LOGD(TAG,"command: %s", command.c_str());
     auto n = known_seq_commands.find(command);
     
     if ( n == known_seq_commands.end() ) {
         return (int) Seq_command::INVALID;
     }
     return (int) n->second;
 //   return (int) Seq_command::INVALID;
}


std::map <unsigned char, char *  > seq_command_map = {
     {0x01, "start recorder"}
    ,{0x02, "stop recorder"}
    ,{0x04, "dump recorder"}
    ,{0x11, "start sequencer"}
    ,{0x12, "stop sequencer"}
    ,{0x13, "rewind sequencer"}
    ,{0x14, "dump recorder"}
    ,{0x21, "start thru"}
    ,{0x22, "stop thru"}
};

    class Outbuf2 : public std::streambuf {
      protected:
	//central output function
	// - print characters in uppercase mode
	 
	virtual int_type overflow (int_type c) {
	    if (c != EOF) {
		// convert lowercase to uppercase
		c = std::toupper(static_cast<char>(c),getloc());

		// and write the character to the standard output
		if (putchar(c) == EOF) {
		    return EOF;
		}
	    }
	    return c;
	}
    }; //Outbuf2


//p. 837 The C++ Standard Library Second Edition, Nicolai M. Josuttis

class Outbuf_buffered_orig : public std::streambuf {
    protected:
        static const int bufferSize = 10; // size of data buffer
        char buffer[bufferSize]; // data buffer
    public:
        // constructor
        // - initialize data buffer
        // - one character less to let the bufferSizeth character cause a call of overflow()
        Outbuf_buffered_orig() {
        setp (buffer, buffer+(bufferSize-1));
        }
        
        // destructor
        // - flush data buffer
        virtual ~Outbuf_buffered_orig() {
        sync();
        }

    protected:
        // flush the characters in the buffer
        int flushBuffer () {
        int num = pptr()-pbase();
        if (write (1, buffer, num) != num) {
        return EOF;
        }
        pbump (-num); // reset put pointer accordingly
        return num;
        }
        
        // buffer full
        // - write c and all previous characters
        virtual int_type overflow (int_type c) {
        if (c != EOF) {
        // insert character into the buffer
        *pptr() = c;
        pbump(1);
        }
        // flush the buffer
        if (flushBuffer() == EOF) {
        // ERROR
        return EOF;
        }
        return c;
        }

        // synchronize data with file/destination
        // - flush the data in the buffer
        virtual int sync () {
        if (flushBuffer() == EOF) {
        // ERROR
        return -1;
        }
        return 0;
        }
};  //Outbuf_buffered

//p. 837 The C++ Standard Library Second Edition, Nicolai M. Josuttis

class Outbuf_buffered_fckx : public std::streambuf {
    protected:
        static const int bufferSize = 128; // size of data buffer
        char buffer[bufferSize]; // data buffer
    public:
        // constructor
        // - initialize data buffer
        // - one character less to let the bufferSizeth character cause a call of overflow()
        Outbuf_buffered_fckx() {
            setp (buffer, buffer+(bufferSize-1));
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "Outbuf_buffered_fckx constructor", 0, 1, 0);
            std::cout << "Outbuf_buffered_fckx constructor " << std::endl;
        }
        
        // destructor
        // - flush data buffer
        virtual ~Outbuf_buffered_fckx() {
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "Outbuf_buffered_fckx destructor", 0, 1, 0);
            //DEBUG
            std::cout << "Outbuf_buffered_fckx destructor " << std::endl;
            sync();
        }

    protected:
        // flush the characters in the buffer
        int flushBuffer () {
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "Outbuf_buffered_fckx flushBuffer", 0, 1, 0);
            int num = pptr()-pbase();

            
           if (num != 0) {
           esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI/flushBuffer", buffer, num-1, 1, 0);  //use num-1 to skip the added endl
           //filter the num = 1 cases in Nodered DIRTY OF THE DIRTIEST
           //also filter num=1 cases that only contain endl
            //if (num < 2) esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI/flushBuffer", buffer, num-1, 1, 0);  //use num-1 to skip the added endl
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ flushBuffer num: " << num << std::endl;
            std::cout << "Outbuf_buffered_fckx _ flushBuffer data: " << buffer << std::endl;
            std::cout << "Outbuf_buffered_fckx _ this was data " <<std::endl;
/*
            if (num != 0) {
                char * dest = "";
                //strncpy(dest, buffer, num);
                //memcpy(dest, buffer, num);
                snprintf(dest, num, "%s", buffer);
                std::cout << "Outbuf_buffered_fckx _ dest " << dest << std::endl;
                std::cout << "Outbuf_buffered_fckx _ this was dest " <<std::endl;
            };
            
*/            
           }
            
            /*
            //Chapter 15: Input/Output Using Stream Classes p. 806
            std::ostringstream os (std::ios::out|std::ios::ate);
            os << 77 << " " << std::hex << 77 << std::endl;
            std::cout << os.str(); // writes: value: 77 4d
            */
            
            //std::cout << std::hex  << buffer << std::endl;
            //https://stackoverflow.com/questions/71660143/where-can-i-find-a-reference-to-the-function-write
            //https://stackoverflow.com/questions/24259640/writing-a-full-buffer-using-write-system-call

            
            if (write (1, buffer, num) != num) {
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ flushBuffer EOF ! (return EOF) num: " << num << std::endl;    
            return EOF;
            }
            pbump (-num); // reset put pointer accordingly
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ flushBufffer NON EOF (return num) num: " << num << std::endl;
            return num;
        }
        
        // buffer full
        // - write c and all previous characters
        virtual int_type overflow (int_type c) {
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "Outbuf_buffered_fckx overflow", 0, 1, 0);
            if (c != EOF) {
           // if (c != 0xa) {  //test on LF      FCKX! 
            // insert character into the buffer
            *pptr() = c;
            pbump(1);
            }
            // flush the buffer
            if (flushBuffer() == EOF) {
            // ERROR
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ overflow ERROR: (return EOF)" << std::endl;
            return EOF;
            }
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ overflow OK (return c): " << c << std::endl;
            return c;
        }

        // synchronize data with file/destination
        // - flush the data in the buffer
        virtual int sync () {
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "Outbuf_buffered_fckx sync", 0, 1, 0);    
            if (flushBuffer() == EOF) {
            // ERROR
            //DEBUG 
            std::cout << "Outbuf_buffered_fckx _ sync ERROR (return -1)" << std::endl;
            return -1;
            }
            //DEBUG
            std::cout << "Outbuf_buffered_fckx _ sync OK (return 0)" << std::endl;
            return 0;
        }
};  //Outbuf_buffered_fckx



    class OutStreamBuf : public std::streambuf { 
      
      protected:
      
        /* central output function
         * - print characters in uppercase mode
        */     
     
      
        //override the existing overflow function with a function that converts each character to uppercase
        virtual int_type overflow (int_type c) {
            if (c != EOF) {
            // convert lowercase to uppercase
            c = std::toupper(static_cast<char>(c),getloc());
            //output to standard output
            putchar(c);
            esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStreamBuf overflow", 0, 1, 0);   

            }
            return c;
        }
       
   
        // write multiple characters  MUST USE CONST CHAR* ?
        virtual std::streamsize xsputn (char* s, std::streamsize num) {  
            std::cout << "**size: " << num << std::endl;    
            std::cout << "OutStreamBuf contents: " << s << std::endl;
            return 1;
        }    
    }; //OutStreamBuf



class OutStream : public std::ostream {
 
    private:
        //private local Outbuf for OutStream
        class Outbuf : public std::streambuf { 
      public:
   
      protected:
	/* central output function
	 * - print characters in uppercase mode
	 */     
     
        //override the existing overflow function with a function that converts each character to uppercase
        virtual int_type overflow (int_type c) {
            if (c != EOF) {
            // convert lowercase to uppercase
            c = std::toupper(static_cast<char>(c),getloc());
            //output to standard output
            putchar(c);
            esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStream inside Outbuf overflow", 0, 1, 0);    //does not show

            }
            return c;
        }
   
        // write multiple characters  MUST USE CONST CHAR* (?)
        virtual std::streamsize xsputn (char* s, std::streamsize num) {  
            std::cout << "**size: " << num << std::endl;    
            std::cout << "OUTBUF contents: " << s << std::endl;
            return 1;
        }    
    }; //Outbuf

        Outbuf outbuf;
        std::streambuf * buf;
     public:
        //constructor
        OutStream() {
        //buf = this->rdbuf();  //compiles OK, but what does it do ?
         buf = rdbuf();         //compiles OK, but what does it do ?  //makes no sense to do it in the constructor. rather do it in operatot <<
         std::cout <<"buf " << buf << " buf" << std::endl;   
         std::cout << "SOME MESSAGE FROM OutStream constructor" <<std::endl;         //shows OK

         esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStream constructor", 0, 1, 0);    //shows OK
        };
        
        // << operator
        //https://www.geeksforgeeks.org/overloading-stream-insertion-operators-c/
        //have a good look on what parameters the operator should take , see the above article       
        friend std::ostream & operator << (std::ostream &stream, const OutStream& outStream){
            
         //buf = rdbuf();         //compiles OK, but what does it do ?  //makes no sense to do it in the constructor. rather do it in operatot <<
         //std::cout <<"buf " << buf << " buf" << std::endl;   
            esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStream << operator", 0, 1, 0); //doesn't show
            stream << "Test1 << operator inside " << std::endl;                                       //doesn't show
            //return stream; //return the stream 
            return (stream << "Test2 << operator inside ");            
        };     
}; //OutStream




 void test_outbuf(void)
    {
    std::cout << "Testing Outbuf" << std::endl;    
	// create special output buffer
	OutStreamBuf ob;
	// initialize output stream with that output buffer
	std::ostream outbuf(&ob);

	outbuf << "31 hexadecimal: "
	    << std::hex << 31 << std::endl;
        
    std::cout << "Tested Outbuf" << std::endl;    
	
    }


class MyNotifier {    
    public:
        //constructor
        MyNotifier(std::ostream& os = std::cout) : ost(os) {}
        //takes eventdata as a code for the event
        //composes some output string based on the input and outputs it to the customizable output stream ost
        virtual void Notify( unsigned long eventdata);
    protected:
        std::ostream& ost;        
}; //class MyNotifier

void MyNotifier::Notify(unsigned long eventdata) {
    //takes eventdata as dummy for an event
    //composes some output string based on the input and outputs it to the customizable output stream ost
    char s[200];
    int wr = sprintf(s, "RECEIVED EVENT %s ", "of type 1 ");
    sprintf( s + wr , "with number %lu\n", eventdata);
    std::cout << "MyNotifier::Notify direct: " << s << std::endl;                     //this shows up
    //ost << "dummy custom_ostream output: " << eventdata << std::endl;
    ost << eventdata << std::endl;  //TRICK  a bit TOO DIRTY to keep ost (Outbuf_buffered_fckx) simple 
    //ost << eventdata;  //DO NOT include an endl !!!  NEEDS REPAIR OF ost !!!
    //trial to send over MQTT,  in the end ost should generate MQTT output
    esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "MyNotifier::Notify", 0, 1, 0); //works fine
} //MyNotifier::Notify


/*
    class Outbuf : public std::streambuf
    {
      protected:
	//central output function
	// - print characters in uppercase mode
	 
	virtual int_type overflow (int_type c) {
	    if (c != EOF) {
		// convert lowercase to uppercase
		c = std::toupper(static_cast<char>(c),getloc());

		// and write the character to the standard output
		if (putchar(c) == EOF) {
		    return EOF;
		}
	    }
	    return c;
	}
    };

*/



class StreamBuf : public std::streambuf {
    
    
    
    
}; //StreamBuf


class Custom_Out : public std::ostream {
    
    
    class Outbuf : public std::streambuf {
      protected:
	//central output function
	// - print characters in uppercase mode
	 
	virtual int_type overflow (int_type c) {
	    if (c != EOF) {
		// convert lowercase to uppercase
		c = std::toupper(static_cast<char>(c),getloc());

		// and write the character to the standard output
		if (putchar(c) == EOF) {
		    return EOF;
		}
	    }
	    return c;
	}
    };
 /*   
    static Outbuf outbuf;
    */  
    public:
        //constructor
      //  Custom_Out &buf;
/*
        {
        //rdbuf(&sb); 
        }
*/        

        
  /*      
         friend std::ostream & operator << (std::ostream &out, const Log &L){
            
              out << "Log test operator ";
              return out; //return the stream
            
        };
        
        
        
        
*/
    private:

        std::streambuf *buf;
        std::ostream out;

        
    //std::streambuf* oldBuf = nullptr;
}; //Custom_Out



class println_stream: std::ostream
{
    // Make this a private class.
    // You don't need to leak implementation details.
    // As far as anybody else is convered it is just std::streambuf
    
    class println_buf { /* STUFF */ };

    println_buf     buf;
    std::streambuf* oldBuf = nullptr;
    public:
        println_stream()
            // You can't pass the buf to the ostream constructor
            // here as it has not been constructed yet.
            // There are ways but lets keep it simple for now.
        {
            // I did not do the work with logfunc
            // Have not read that bit yet.
            // But you get the idea I hope.
            //oldBuf = rdbuf(&buf);   //<<<< throws an error
        }
        ~println_stream()
        {
            // Can't remember if it is required.
            // But I would always set back the original buffer.
            rdbuf(oldBuf);
        }
};


//https://codereview.stackexchange.com/questions/167321/custom-output-stream-that-acts-like-stdcout

/*
namespace Utils
{
    static std::ostream& Log;
}
*/

struct MyStream1 : std::ostream, std::streambuf
{
public:
   MyStream1() : std::ostream(this) { }
   std::streambuf::int_type overflow(std::streambuf::int_type c)
   {
      std::cout << c;
      // My custom operations
      return 0;
   }
};
//std::ostream& Utils::Log = MyStream1();


struct MyStream2 : std::streambuf, std::ostream 
{
public:
   MyStream2() : std::ostream(this) { }
   std::streambuf::int_type overflow(std::streambuf::int_type c) override
   {
      std::cout << c;
      // My custom operations
      return 0;
   }
};
//std::ostream& Utils::Log = MyStream2();


class MyStream3 : std::ostream, std::streambuf
{
public:
   MyStream3() : std::ostream(this) { }
   std::streambuf::int_type overflow(std::streambuf::int_type c)
   {
      std::cout << c;
      // My custom operations
      return 0;
   }
};

class MyStream4 : std::streambuf, std::ostream 
{
public:
   MyStream4() : std::ostream(this) { }
   std::streambuf::int_type overflow(std::streambuf::int_type c) override
   {
      std::cout << c;
      // My custom operations
      return 0;
   }
};


class MQTT_stream: public std::ostream
{
    // Make this a private class.
    // You don't need to leak implementation details.
    // As far as anybody else is convered it is just std::streambuf
    class MQTT_buf : public std::streambuf { /* STUFF */ };

    MQTT_buf      buf;
    std::streambuf* oldBuf = nullptr;
    public:
        MQTT_stream()
            // You can't pass the buf to the ostream constructor
            // here as it has not been constructed yet.
            // There are ways but lets keep it simple for now.
        {
            // I did not do the work with logfunc
            // Have not read that bit yet.
            // But you get the idea I hope.
            oldBuf = rdbuf(&buf);   //<<<< throws an error
            
            //send buffer to MQTT with the right topic
            //...code here
            //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "test", 0, 1, 0);
            std::cout << "Hello Back!";
            std::cout << oldBuf;
        }
        ~MQTT_stream()
        {
            // Can't remember if it is required.
            // But I would always set back the original buffer.
            rdbuf(oldBuf);
        }
};


class Log : public std::ostream
{
    public:
        enum Mode { STDOUT, FILE };

        // Needed by default
        Log(const char *file = NULL);
        ~Log();

        // Writing methods
        void write(char *);
        void write(std::string);
        void write_mqtt(char *);
        
        //https://www.geeksforgeeks.org/overloading-stream-insertion-operators-c/
        /*
        ostream & operator << (ostream &out , const Complex &c)
            {
                out << c.real;
                out << "+i" << c.imag << endl;
                return out;
            }
        */        
        friend std::ostream & operator << (std::ostream &out, const Log &L){
            
              out << "Log test operator ";
              return out; //return the stream
            
        };        
        
    private:
        Mode mode;
        std::streambuf *buf;
        std::ofstream of;
        std::ostream out;
};

Log::Log(const char *file) : out(nullptr)
{
    if (file != NULL)
    {
        of.open(file);
        buf = of.rdbuf();
        mode = FILE;
        
    }
    else
    {
        buf = std::cout.rdbuf();
        mode = STDOUT;
    }

    // Attach to out
    out.rdbuf(buf);
  //  esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "test Log constructor", 0, 1, 0);
     write(">>>>>>>>>>>>>>>>>>>>>>>>test Log write in constructor<<<<<<<<<<<<<<<<<<<<<<<<<<"); //works
}

Log::~Log()
{
    if (mode == FILE)
        of.close();
}

void Log::write(std::string s)
{
    out << "Log out 1:" << s << std::endl;
}
void Log::write(char *s)
{
    out << "Log out 2:" << s << std::endl;     
}


void Log::write_mqtt(char *s)
{   
    esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "test Log::write_mqtt", 0, 1, 0);   
}


//Log out;

/*
//implementation of << operator
std::ostream & operator << (std::ostream &out, Log &buf)
    {
        out << "Log test operator ";
        out << "+i" << " something " << std::endl;
        return out;
    }
*/




class MyBuf1 : public std::stringbuf
{
//https://stackoverflow.com/questions/13703823/a-custom-ostream    
public:
    virtual int sync() {
        ESP_LOGV(TAG,"MYBUF1");
        std::cout << "TEST MyBuf" << std::endl;
        // add this->str() to database here
        // (optionally clear buffer afterwards)
        return 0;
    }
};




 //  MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier, all default pars


    Outbuf_buffered_fckx outbuf_buffered_fckx;
    std::ostream custom_ostream(&outbuf_buffered_fckx);  //OK 
    
    MIDISequencerGUINotifierRaw raw_n(0,custom_ostream);


//MIDISequencerGUINotifierText text_n;
 //  MIDISequencerGUINotifierText text_n(0, custom_ostream);
//   MIDISequencerGUINotifierRaw raw_n;
   
   //MIDISequencerGUINotifierText text_n(0, std::cout);        // the AdvancedSequencer GUI notifier, with pars, but default
   //MQTT_stream mqtt_stream;
   //MIDISequencerGUINotifierText text_n(0, mqtt_stream);        // the AdvancedSequencer GUI notifier
    //MIDISequencerGUINotifierText text_n(0, new MQTT_stream);        // the AdvancedSequencer GUI notifier
  
  
 /*  
   AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)
   MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX
*/


AdvancedSequencer *ptrAdvancedSequencer;
MIDIRecorder *ptrMIDIRecorder;

MIDIThru* thru;                  // a MIDIThru ORIG
MIDIProcessorPrinter printer;    // a MIDIProcessor which prints passing MIDI events  

//https://stackoverflow.com/questions/71329110/how-to-make-a-set-of-mutually-dependent-classes-globally-accessible

//AdvancedSequencer ptrAdvancedSequencer(&text_n);
   //MIDIRecorder ptrMIDIRecorder(&ptrAdvancedSequencer); 
//void * sequencer_p
//void * recorder_p;


void init_test_recorder( void ) {
    
    
    std::cout << "INIT_TEST_RECORDER" << std::endl;  
 
     //Outbuf2 outbuf2;
     //std::ostream custom_ostream(&outbuf2);    //OK
 
     //MyBuf1 myBuf1;
     //std::ostream custom_ostream(&myBuf1);  // NOK
 
 
    //Outbuf_buffered_fckx outbuf_buffered_fckx; allready intitialized
    std::ostream custom_ostream(&outbuf_buffered_fckx);  //OK 
 
  //  OutStreamBuf outStreamBuf;                                     //Stackoverflow 1       
  //  std::ostream custom_ostream(&outStreamBuf);  //OK
     
    //OutStream custom_ostream;   //using a composite ostream/streambuf object    generates a log from the constructor   //Stackoverflow 2
                                //it's output is not shown
     
     
    // MyNotifier notifier;                    //OK instantiate with default output stream (std::cout)
  // MyNotifier notifier(std::cout);         //OK instantiate with default output stream explicitly

     
     
    MyNotifier notifier(custom_ostream);  //instantiate with customized output stream , 
                                          //NOK when custom_stream uses MyBuf1 or Outbuf,  it works with std:cout   THIS IS STUFF FOR Stackoverflow


notifier.Notify(12345678);
notifier.Notify(56789);
//              
notifier.Notify(0xffffffff); //max 4 byte value
          //max 4294967295



  //  notifier.Notify(123456789);  //notify some event  
  //  notifier.Notify(827364536);  //notify some event 
    //custom_ostream << "Test << operator" << std::endl; 
  
     /*
    Custom_Out custom_Out; 
    MyNotifier notifier(custom_Out); //instantiate with custom output stream, the custom_Out needs to be instantiated with a parameter pointing at streambuf, but may be nullptr. (??)
  notifier.Notify(7272413);  //notify some event       
  */     
       

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>  test_outbuf();
    
    
//   mqtt_stream << "hello world!";
   // out << "hello Log!";
   //Log log;
   //std::cout << log;

//std::cout << out;
   
   //std::cout << "TEST LOG";
   //log.write("hello Log!");
   //esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "test outside log", 0, 1, 0);
   //log.write_mqtt("test log.write_mqtt");
   //std::cout << "testing feed to Log with <<" << std::endl;
   //log << "Log log test stream"; //doesn't trigger output
   //out << "Log out test stream"; //doesn't trigger output 
   //std::cout << "TEST LOG";
  //  std::ostream& Utils::Log = MyStream1();

    ESP_LOGE(TAG,"Entering test_recorder of TEST_RECORDER example");
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER");
 


    //ptrAdvancedSequencer = new AdvancedSequencer(&text_n);
    ptrAdvancedSequencer = new AdvancedSequencer(&raw_n);
    ptrMIDIRecorder = new MIDIRecorder(ptrAdvancedSequencer);  
    MIDIManager::OpenInPorts();          //FCKX!! 220103 must be closed at end
    MIDIManager::OpenOutPorts();            //must be closed at end 
    setMidiCharacteristicCallBacks();
    
    
    
    //ESP_LOGE(TAG,"Going to create GUI Characteristic");
    //createGUICharacteristic(); //NEW NEW
    
//#define ENABLEGUICHAR 
#ifdef ENABLEGUICHAR
    setGuiCharacteristicCallBacks();
    ESP_LOGE(TAG,"Finished creating GUI Characteristic");
#endif //ENABLEGUICHAR
    
    MIDITimer::SetResolution(1*portTICK_PERIOD_MS); //for ESP32 resolution must be a multiple of the system tick
    //keep this for a while to detect if resolution is properly implemented in ESP32_TIMER case
                                  //setting it to 50 leads to timing errors in recordings?
    //could create a waiting loop for availability of port(s)
    //port->isPortOpen())
    /*
    while (true) {
    ESP_LOGE(TAG,"TEST IF PORT IS OPEN %d",MIDIManager::GetOutDriver(0)->IsPortOpen());
    }
    */

    ESP_LOGE(TAG,"PROCEEDING with TEST_RECORDER example (a connection should be available here, because tested before)");

    //MIDIManager::AddMIDITick(&recorder);
MIDIManager::AddMIDITick(ptrMIDIRecorder); 
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //text_n.SetSequencer(&sequencer);      // This is already called by AdvancedSequencer constructor
       //text_n.SetSequencer(ptrAdvancedSequencer);      // This is already called by AdvancedSequencer constructor
    try {
        ESP_LOGE(TAG,"thru = new MIDIThru;");     
        thru = new MIDIThru;  //ORIG    
        thru->SetInPort(0); //FCKX  select MQTT port, it should exist
        thru->SetOutPort(0); //FCKX  select nimBLE port, it should exist

        //must select channel
        //NOTE: -1 does not work

        thru->SetInChannel(-1); //FCKX select only inputs on channel 0
        thru->SetOutChannel(-1); //FCKX select only inputs on channel 0
    }
    catch( ... ) {
    
        ESP_LOGE(TAG,"The MIDIThru constructor throws an exception if in the system are not present almost a MIDI in and a MIDI out ports!");         
    //return 0;
    }

    // adds the thru to the MIDIManager queue
    MIDIManager::AddMIDITick(thru);

    // plugs the MIDIProcessorPrinter into the metronome, so all MIDI messages will be printed to stdout
    thru->SetProcessor(&printer);
    // sets the printing of passing events on and off
    printer.SetPrint(true);   
   
    //implement commands, and call them to trigger user commands:
    //see:  handle_seq_command dispatcher 
    
    
        ESP_LOGE(TAG,"Entering Idle Loop of test_recorder");
        ESP_LOGE(TAG,"USE GUI TO ISSUE COMMANDS");
    /*    
        while(1) {
            //ESP_LOGE(TAG,"Idle Loop of test_recorder");
             vTaskDelay(10 / portTICK_PERIOD_MS);  
        };
        ESP_LOGE(TAG,"End of test_recorder");
    */
}; //init_test_recorder


void handle_seq_command(esp_mqtt_event_handle_t event){
    static const char *TAG = "HANDLE_SEQ_COMMAND_IN"; 
    ESP_LOGD(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
    ESP_LOGD(TAG,"DATA:%.*s ", event->data_len, event->data);
    ESP_LOGD(TAG,"data_len %d",event->data_len); 
    char * command =   event->data;
    
    switch( command[0]){
        case 0x01: 
        {
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
            
            MIDIClockTime t = ptrAdvancedSequencer->MeasToMIDI(15,0); //endMeasure, endBeat record the first 6 beats FCKX            
            ptrMIDIRecorder->SetEndRecTime(t);
            ptrMIDIRecorder->EnableTrack(1); //FCKX
            ptrMIDIRecorder->SetTrackRecChannel(1,-1);      // Can you set this? YES Otherwise set a specific channel
            ptrMIDIRecorder->Start();             
            std::cout << "Recorder started\n";
            thru->Start();
            std::cout << "Thru started\n";
            
        break; }    
        case 0x02:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
                        ptrMIDIRecorder->Stop();
            std::cout << "Recorder stopped\n";      //never reached after calling 
            break;
        case 0x04:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
            DumpMIDIMultiTrackWithPauses(ptrMIDIRecorder->GetMultiTrack()); //in functions.cpp
            std::cout << "Recorder stopped\n";      //never reached after calling 
            break;            
        case 0x11:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
                        ptrAdvancedSequencer->Play();
            std::cout << "Sequencer started at measure: " << ptrAdvancedSequencer->GetCurrentMeasure() << ":"
                 << ptrAdvancedSequencer->GetCurrentBeat() << std::endl;

            break;
        case 0x12:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
                        ptrAdvancedSequencer->Stop();
            std::cout << "Sequencer stopped at measure: " << ptrAdvancedSequencer->GetCurrentMeasure() << ":"
                 << ptrAdvancedSequencer->GetCurrentBeat() << std::endl;
            break; 
        case 0x13:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE IMPLEMENTED", command[0], command[0], seq_command_map[command[0]]); 
            ptrAdvancedSequencer->GoToZero();
            std::cout << "Rewind to 0:0" << std::endl;
            break;   
        case 0x14:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
            DumpMIDIMultiTrackWithPauses(ptrAdvancedSequencer->GetMultiTrack()); //in functions.cpp
            std::cout << "Recorder stopped\n";      //never reached after calling 
            break; 
        case 0x21:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
            thru->Start();
            std::cout << "Thru started\n";
            break;
        case 0x22:
            ESP_LOGD(TAG,"COMMAND:%d (0x%X) %s TO BE TESTED", command[0], command[0], seq_command_map[command[0]]); 
            thru->Stop();
            std::cout << "Thru stopped\n";
            break;            
        default:
            ESP_LOGE(TAG,"UNKNOWN COMMAND: %d (0x%X)", command[0], command[0]);
            break;
} //switch(command[0])

};

#endif //TEST_RECORDER 

void handle_midi_single_in(esp_mqtt_event_handle_t event){
    static const char *TAG = "HANDLE_MIDI_SINGLE_IN"; 
       
    //receive a 5 byte MIDI message 

    //display incoming MIDI data
    ESP_LOGD(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
    //ESP_LOGD(TAG,"DATA:%.*s ", event->data_len, event->data);
    ESP_LOGD(TAG,"data_len %d",event->data_len);  

    ESP_LOGD(TAG,"DATA0 %d (0x%X)", event->data[0], event->data[0]);
    ESP_LOGD(TAG,"DATA1 %d (0x%X)", event->data[1], event->data[1]);
    ESP_LOGD(TAG,"DATA2 %d (0x%X)", event->data[2], event->data[2]); 
    ESP_LOGD(TAG,"DATA3 %d (0x%X)", event->data[3], event->data[3]);
    ESP_LOGD(TAG,"DATA4 %d (0x%X)", event->data[4], event->data[4]);

//#define TRAPEDIT    
#ifdef TRAPEDIT   //for testing
    
    if ((event->data[2] == 0xB0) && (event->data[3] == 0x70)) {
      ESP_LOGD(TAG,"Trapped START EDITING COMMAND"); 
      //for testing send syncing command to GUI controls 
      const char * controllerVal = "32";
            esp_mqtt_client_publish(event->client, "/fckx_seq/controller/A", controllerVal, 0, 1, 0);     
       
        };

#endif
    
    //prepare for immediate sending to sound board (MIDI THRU)
    //create midiPacket for BLE notify
    uint8_t midiPacket[] = {
    event->data[0], //header
    event->data[1], //timestamp, not implemented
    event->data[2], //status
    event->data[3], //== 60 == middle c
    event->data[4] //velocity
        };//
   
 
    double time = 111; //dummy timestamp for MIDIBLE packet
    
    void* p = MIDIManager::GetInDriver(0);  //when called in the driver this should be "this"

    std::vector<unsigned char> msg_bytes_data = {};
    std::vector<unsigned char>* msg_bytes = &msg_bytes_data; 
    /*
    std::vector<unsigned char> msg_bytes_data =  { 0x10, 0x20, 0x30 };    
    std::vector<unsigned char>* msg_bytes = &msg_bytes_data; 
    */
    
    //msg_bytes_data =  { 0x90, 0x20, 0x7f };
    msg_bytes_data =  { event->data[2], event->data[3], event->data[4] };
    //push incoming message to input queue.  Normally used as a callback on input port
    MIDIManager::GetInDriver(0)->HardwareMsgIn(time,msg_bytes, p);  //should contain msg_bytes_data!!!!


    //development helper to check analyze proper operation of the private data object        
    //MIDIManager::GetInDriver(0)->printData();
    ESP_LOGV(TAG,"Learning about MIDIManager Interface"); 
    ESP_LOGV(TAG,"MIDIManager::GetNumMIDIIns() %d",MIDIManager::GetNumMIDIIns());          //FCKX
    ESP_LOGV(TAG,"MIDIManager::GetInDriver(0)->GetQueueSize() %d",MIDIManager::GetInDriver(0)->GetQueueSize()); 
    ESP_LOGV(TAG,"MIDIManager::GetInDriver(0)->CanGet() %d",MIDIManager::GetInDriver(0)->CanGet()); 
  
    dumpflag = true;
    //if MIDI THRU
    //send midiPacket immediately to sound board
    //add check on NimBLE connection
    
    
    //switch of thru
    //pCharacteristic->setValue(midiPacket, 5);
   // pCharacteristic->notify();       
       
};


//#ifdef NIMBLE_IN_MAIN   this is only because of midi thru!!!!!  so switch off thru

static void call_fckx_seq_api(esp_mqtt_event_handle_t event){

    //receive a 5 byte array 
    static const char *TAG = "FCKX_SEQ_API"; 
    //MIDI COMMANDS               
    if (strncmp(event->topic, "/fckx_seq/midi/single",strlen("/fckx_seq/midi/single")) == 0) {
   
        handle_midi_single_in(event);   
          }        
    else
    //TESTER        
    if (strncmp(event->topic, "/fckx_seq/midi/test",strlen("/fckx_seq/midi/test")) == 0) {
        std::cout << "TEST (event->data): " << event->data << std::endl;
        std::cout << "TEST (event->data_len): " << event->data_len << std::endl;
              // std::cout << e << std::endl;
        //ESP_LOGD(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
        //ESP_LOGD(TAG,"DATA:%.*s ", event->data_len, event->data);
        //ESP_LOGD(TAG,"data_len %d",event->data_len);  
        ESP_LOGI(TAG,"COMMAND:%.*s\r ", event->topic_len, event->topic);
        ESP_LOGI(TAG,"...command to be implemented...");      
        } 
#ifdef TEST_RECORDER        
    else
    //NONE MIDI COMMANDS (e.g. for NiCMidi MIDIManager 

    if (strncmp(event->topic, "/fckx_seq/command",strlen("/fckx_seq/command")) == 0) {
        ESP_LOGI(TAG,"COMMAND:%.*s\r ", event->topic_len, event->topic);
        handle_seq_command(event);
        ESP_LOGI(TAG,"...command to be implemented...");      
        } 
#endif // TEST_RECORDER       
    else {
         ESP_LOGI(TAG,"COMMAND:%.*s\r ", event->topic_len, event->topic);
        ESP_LOGI(TAG,"unknown API command");}
};
// #endif
 

/*
* MQTT functionality
*/
int msg_id;
/*
* MQTT callback based on the code in ESP-IDF example xxxxxxxxxx
*/
static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event){ 
    static const char *TAG = "mqtt_event_handler_cb"; 
    //ESP_LOGI(TAG, "FCKX: HANDLER_CB CALLED");  //FCKX
    esp_mqtt_client_handle_t client = event->client;
    //int msg_id;
    int result;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            

            
            msg_id = esp_mqtt_client_publish(client, "/fckx_seq/test/qos1", "test qos1", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/faust/test/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq/test/qos1", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/fckx_seq/test/qos1");
            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/Polyphonic/Voices/#",0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
 
            msg_id = esp_mqtt_client_subscribe(client, "/wm8978/#",0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);             
            
            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq/command/#", 0);
            //ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            //msg_id = esp_mqtt_client_publish(client, "/fckx_seq", "MQTT OK", 0, 1, 0);
            //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq/midi/single",1);
            esp_mqtt_client_subscribe(client, "/fckx_seq/command",1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq/midi/test",0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);            
            
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGE(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/fckx_seq/test/qos0", "MQTT EVENT SUBSCRIBED", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            //ESP_LOGI(TAG, "TOPIC=%.*s\r", event->topic_len, event->topic);
            //ESP_LOGI(TAG,"DATA=%.*s\r", event->data_len, event->data);
            //if topic starts with /fckx_seq/api or /fckx_seq/api2, call a command dispatcher/handler
   /*
            if (strncmp(event->topic, "/fckx_seq/api2",strlen("/fckx_seq/api2")) == 0) {
                call_faust_api2(event);    
                }
            else
*/
/*
                if (strncmp(event->topic, "/fckx_seq/api",strlen("/fckx_seq/api")) == 0) {
                call_faust_api(event);
                }                
            else
*/

                if (strncmp(event->topic, "/fckx_seq/",strlen("/fckx_seq/")) == 0) { //this means: string starts with ....
                               call_fckx_seq_api(event);   }; 
                 

  

/*
                if (strncmp(event->topic, "/Polyphonic/",strlen("/Polyphonic/")) == 0) { //this measn: string starts with ....
                if (!call_auto_faust_api(event)) { //go for checking additional commands only if no DspFaust address handled
                call_faust_api(event);   }; 
                } 
            else if (strncmp(event->topic, "/wm8978/",strlen("/wm8978/")) == 0) {
                call_faust_api2(event);  
                
                }  
*/                
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
           // ESP_LOGE(TAG, "Switching allNotesOff to prevent future crash");
           // DSP->allNotesOff();
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

/*
* MQTT event handler based on the code in ESP-IDF example xxxxxxxxxx
* throws a compilation error! May be obsolete when registering is done i a different way
*/
/*
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {

    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}
*/

/* MQTT event handler based on the code in ESP-IDF example xxxxxxxxxx
* repair by FCKX of the original code, see above
*/
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, esp_mqtt_event_handle_t event_data) {
    static const char *TAG = "mqtt_event_handler"; 
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

/*
../main/main.cpp: In function 'void mqtt_event_handler(void*, esp_event_base_t, int32_t, void*)':
../main/main.cpp:94:27: error: invalid conversion from 'void*' to 'esp_mqtt_event_handle_t' {aka 'esp_mqtt_event_t*'} [-fpermissive]
     mqtt_event_handler_cb(event_data);
                           ^~~~~~~~~~
*/

/*
//https://github.com/espressif/esp-idf/issues/5248

@mntolia This is an issue with C++ build, has been fixed in espressif/esp-mqtt@8a1e1a5, but apparently that hasn't made it yet to arduino. As a workaround you can cast the variables to the expected types or even avoid using an event loop altogether.
It is still possible to configure mqtt_event_handler_cb() as a plain callback in client config:

    mqtt_config.event_handle = mqtt_event_handler_cb;
And remove both definition of the mqtt_event_handler() and registration of the handler esp_mqtt_client_register_event()

*/

/* MQTT client based on the code in ESP-IDF example ...\esp-idf\examples\protocols\mqtt\tcp
*  some adaptations were necessary
*/
static esp_mqtt_client * mqtt_app_start(void){
//static void mqtt_app_start(void){
   
    //FCKX
    esp_mqtt_client_config_t mqtt_cfg = {0}; // adapted by FCKX, see above
    mqtt_cfg.uri = SECRET_ESP_MQTT_BROKER_URI;
    mqtt_cfg.username = SECRET_ESP_MQTT_BROKER_USERNAME;
    mqtt_cfg.password = SECRET_ESP_MQTT_BROKER_PASSWORD;
    mqtt_cfg.client_id = SECRET_ESP_MQTT_CLIENT_ID;
    
#if CONFIG_BROKER_URL_FROM_STDIN
    char line[128];

    if (strcmp(mqtt_cfg.uri, "FROM_STDIN") == 0) {
        int count = 0;
        printf("Please enter url of mqtt broker\n");
        while (count < 128) {
            int c = fgetc(stdin);
            if (c == '\n') {
                line[count] = '\0';
                break;
            } else if (c > 0 && c < 127) {
                line[count] = c;
                ++count;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        mqtt_cfg.uri = line;
        printf("Broker url: %s\n", line);
    } else {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }
#endif /* CONFIG_BROKER_URL_FROM_STDIN */

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    //mqtt_cfg.event_handle = mqtt_event_handler_cb;
    //esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, (esp_event_handler_t)mqtt_event_handler, client);  //FCKX, added 2x type casting 
    esp_mqtt_client_start(client);
    ESP_LOGI(TAG, "MQTT CLIENT STARTED");  //FCKX
    
    return client;
}

/*
* END OF MQTT functionality
*/


//#include "../include/tick.h"
//#include "../include/manager.h"
#include "../include/metronome.h"


using namespace std;


#ifdef TEST_ADVANCEDSEQUENCER

ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_ADVANCEDSEQUENCER");
MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier

/*******************************************************************************************
* AdvancedSequencer also instantiates nimBLEDevice (via sequencer, manager, driver, RtMidi)
********************************************************************************************/
AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)
                        
MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX
#endif


#ifdef TEST_THRU

//GLOBALS for test_thru example

MIDIThru* thru;                  // a MIDIThru ORIG
MIDIProcessorPrinter printer;    // a MIDIProcessor which prints passing MIDI events                 



int test_thru(  ) {

//init port here. this is not in the original code
    //MIDIManager::OpenInPorts();
try {
    ESP_LOGE(TAG,"thru = new MIDIThru;");         


    thru = new MIDIThru;  //ORIG
    thru->SetInPort(0); //FCKX  select MQTT port, it should exist
    thru->SetOutPort(0); //FCKX  select nimBLE port, it should exist
  
//must select channel
//NOTE: -1 does not work

  thru->SetInChannel(-1); //FCKX select only inputs on channel 0
  thru->SetOutChannel(-1); //FCKX select only inputs on channel 0
 

}
catch( ... ) {
    
    ESP_LOGE(TAG,"The MIDIThru constructor throws an exception if in the system are not present almost a MIDI in and a MIDI out ports!");         
    return 0;
}

    // adds the thru to the MIDIManager queue
    MIDIManager::AddMIDITick(thru);

    // plugs the MIDIProcessorPrinter into the metronome, so all MIDI messages will be printed to stdout
    thru->SetProcessor(&printer);
    // sets the printing of passing events on and off
    printer.SetPrint(true); 
    setMidiCharacteristicCallBacks();
  
  return 1;
 
} //test_thru(  )
#endif // #ifdef TEST_THRU


#ifdef TEST_COMPONENT  
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_COMPONENT");
//GLOBALS for test_component example
//NOTE: required includes are not in this block

 /*
 *   Example file for NiCMidi - A C++ Class Library for MIDI
 *
 *   Copyright (C) 2021  Nicola Cassetta
 *   https://github.com/ncassetta/NiCMidi
 *
 *   This file is part of NiCMidi.
 *
 *   NiCMidi is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free 
 Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   NiCMidi is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with NiCMidi.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
  Example of a basic custom MIDITickComponent which only plays a note
  every second. The file shows how to redefine the base class methods
  and how to add the component to the MIDIManager queue, making it
  effective.
*/



class TestComp : public MIDITickComponent {
    public:
                                TestComp();
        virtual void            Reset() {}
        virtual void            Start();
        virtual void            Stop();

    protected:
        static void             StaticTickProc(tMsecs sys_time, void* pt);
        virtual void            TickProc(tMsecs sys_time);


        static const tMsecs     NOTE_INTERVAL = 1000;   // the time (in msecs) between two note on
        static const tMsecs     NOTE_LENGTH = 400;      // the time between note on and note off

        tMsecs                  next_note_on;
        tMsecs                  next_note_off;
};


// The constructor must call the base class constructor with the priority and the StaticTickProc
// pointer as parameters.
// In our case, as we have only one object in the MIDIManager queue, the priority is irrelevant.
//

TestComp::TestComp() : MIDITickComponent(PR_PRE_SEQ, StaticTickProc) {}


// The Start() method should initialize the class and then call the base class Start(),
// which loads the sys_time_offset variable with the start time and enables the callback.
//

void TestComp::Start() {
    cout << "Starting the component ... " << endl;
    // opens MIDI out 0 port before playing the notes
    MIDIManager::GetOutDriver(0)->OpenPort();
    next_note_on = 0;                   // relative time of the 1st note on
    next_note_off = NOTE_LENGTH;        // relative time of the 1st note off
    MIDITickComponent::Start();
}


// The Stop() method should first call the base class Stop() which disables the callback.
//
void TestComp::Stop() {
    MIDITimedMessage msg;
    msg.SetNoteOff(0, 36, 0);
    MIDIManager::GetOutDriver(0)->OutputMessage(msg); //FCKX added a final note off befor stopping the component

    MIDITickComponent::Stop();
    cout << "Stopping the component ... " << endl;
    //MIDIManager::GetOutDriver(0)->ClosePort();
}


// This is the typical implementation of the static callback.
// The MIDIManager gives it two parameters: the now absolute time (sys_time) and the object
// "this" pointer (as void*); the callback only should cast the void pointer to a class
// pointer and then call the (virtual) derived class callback (i.e. TickProc).
//
void TestComp::StaticTickProc(tMsecs sys_time, void* pt) {
    TestComp* c_pt = static_cast<TestComp*>(pt);
    c_pt->TickProc(sys_time);
}


// This is finally the object callback, which does all the work. Its parameter is the absolute
// now time (remember you have the start time in the sys_time_offset variable).
//
void TestComp::TickProc(tMsecs sys_time) {
    MIDITimedMessage msg;
    tMsecs deltat = sys_time - sys_time_offset; // the relative time (now time - start time)

    if (deltat >= next_note_off) {              // we must turn off the note
        msg.SetNoteOff(0, 36, 0);
        MIDIManager::GetOutDriver(0)->OutputMessage(msg);
                                                // sends a note off message to the MIDI 0 port
        //sendToMIDIOut(msg);                         // sends a note off message to the BLE interface               
        cout << "and off" << endl;
        next_note_off += NOTE_INTERVAL;         // updates the next note off time
    }

    if (deltat >= next_note_on) {               // we must turn on the note
        msg.SetNoteOn(0, 36, 127);
        MIDIManager::GetOutDriver(0)->OutputMessage(msg);  //phase in
                                                // sends a note on message to the MIDI 0 port
    //    sendToMIDIOut(msg);            //phase out             // sends a note off message to the BLE interface                                                                                                                                                                                      
        cout << "Note on ... ";
        next_note_on += NOTE_INTERVAL;          // updates the next note on time
    }
    
    /*
     //third event added by FCKX
    if (deltat >= next_note_off) {              // we must turn off the note
        msg.SetNoteOff(0, 36, 0);
        MIDIManager::GetOutDriver(0)->OutputMessage(msg);
                                                // sends a note off message to the MIDI 0 port
        //sendToMIDIOut(msg);                   // sends a note off message to the BLE interface               
        cout << "...  and off" << endl;
        next_note_off += NOTE_INTERVAL;         // updates the next note off time
    } */
    
}

// The main() creates a class instance, adds it to the MIDIManager queue and then calls
// Start() and Stop() for enabling and disabling the callback
int main_test_component() {
    static const char *TAG = "TEST_COMPONENT"; 
    TestComp comp;                              // creates the component
    MIDIManager::AddMIDITick(&comp);            // adds it to the MIDIManager queue
    
    //some trials to learn about the MIDIManager interfae and indrivers
    ESP_LOGE(TAG,"Learning about MIDIManager Interface"); 
    ESP_LOGE(TAG,"MIDIManager::GetNumMIDIIns() %d",MIDIManager::GetNumMIDIIns());          //FCKX
    ESP_LOGE(TAG,"MIDIManager::GetInDriver(0)->GetQueueSize() %d",MIDIManager::GetInDriver(0)->GetQueueSize()); 
    ESP_LOGE(TAG,"MIDIManager::GetInDriver(0)->CanGet() %d",MIDIManager::GetInDriver(0)->CanGet());

    
    comp.Start();                               // starts the callback
    cout << "Waiting 10 secs ... " << endl;
    MIDITimer::Wait(10000);                     // waits 10 secs
    comp.Stop();                                // stops the callback
    cout << "Waiting 5 secs without playing ... " << endl;
    MIDITimer::Wait(5000);                      // waits 5 secs
    cout << "Exiting" << endl;
    return EXIT_SUCCESS;
}

    /**********************************************************************************
    * END OF TEST  NiCMidi functionality   test_component.cpp
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/    
#endif 

#ifdef TEST_RECORDING_NO_INPUT  //see issue #6 NiCMidi repo


void test_recording_no_input( void ) {

    static const char *TAG = "TEST_RECORDING_NO_INPUT"; 
    ESP_LOGE(TAG,"Entering test_recording_no_input of TEST_RECORDING_NO_INPUT example");
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDING_NO_INPUT");
    MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier
    AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)
    MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX
    MIDIManager::OpenInPorts();          //FCKX!! 220103 must be closed at end
    MIDIManager::OpenOutPorts();            //must be closed at end
 
    //MIDITimer::Wait(5000); 
    MIDITimer::SetResolution(1*portTICK_PERIOD_MS); //for ESP32 resolution must be a multiple of the system tick
    //keep this for a while to detect if resolution is properly implemented in ESP32_TIMER case
                                  //setting it to 50 leads to timing errors in recordings?
    //could create a waiting loop for availability of port(s)
    //port->isPortOpen())
    /*
    while (true) {
    ESP_LOGE(TAG,"TEST IF PORT IS OPEN %d",MIDIManager::GetOutDriver(0)->IsPortOpen());
    }
    */

    ESP_LOGE(TAG,"PROCEEDING with TEST_RECORDING_NO_INPUT example (a connection should be available here, because tested before)");

    MIDIManager::AddMIDITick(&recorder);
    //text_n.SetSequencer(&sequencer);      // This is already called by AdvancedSequencer constructor



    MIDIClockTime t = sequencer.MeasToMIDI(15,0); //endMeasure, endBeat record the first 6 beats
    recorder.SetEndRecTime(t);
    recorder.EnableTrack(1); //FCKX

    recorder.SetTrackRecChannel(1,-1);      // Can you set this? YES Otherwise set a specific channel
    //recorder.SetTrackRecChannel(1,-1);  
    recorder.Start();
    std::cout << "Recorder started\n";
   
    MIDITimer::Wait(35000);                 // Waits 15 secs: play something to record (remember to match
                                            // the input channel with the one set in SetTrackRecChannel)
    std::cout << "Calling recorder.Stop()\n";
    recorder.Stop();
    std::cout << "Recorder stopped\n";      //never reached after calling recorder.Stop()
  
    MIDITimer::Wait(1000); 
    std::cout << "Proceeding with next steps\n";
    sequencer.GoToZero();                   // rewinds
    sequencer.Start();
    std::cout << "Now the sequencer plays what you have recorded\n";

    while (sequencer.IsPlaying())         // waits until the sequencer finishes
        MIDITimer::Wait(50);
        
    ESP_LOGE(TAG,"End of TEST_RECORDING_NO_INPUT CODE");    
}
#endif //TEST_RECORDING_NO_INPUT


#ifdef TEST_ADVANCEDSEQUENCER_NOINPUT

/*
 *   Example file for NiCMidi - A C++ Class Library for MIDI
 *
 *   Copyright (C) 2021  Nicola Cassetta
 *   https://github.com/ncassetta/NiCMidi
 *
 *   This file is part of NiCMidi.
 *
 *   NiCMidi is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   NiCMidi is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with NiCMidi.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
  A nice little example demonstrating how to edit the MIDIMultitrack
  embedded in an AdvancedSequencer, play its content and then save
  it in a MIDI file.
*/


#include <string>

#include "../include/advancedsequencer.h"
#include "../include/filewritemultitrack.h"     // for WriteMIDIFile() function

using namespace std;


// A struct holding data for a note
struct note_data {
    unsigned char   note;
    MIDIClockTime   length;
    MIDIClockTime   time;
};


//////////////////////////////////////////////////////////////////
//                        G L O B A L S                         //
//////////////////////////////////////////////////////////////////


// The default ctor creates an AdvancedSequencer with 17 empty tracks (1 master + 16 channel)
//AdvancedSequencer sequencer; //move into the proc to prevent early initialisation of nimBLE ?


// Data for "Twinkle twinkle", track 1
int track1_len = 14;
note_data track1[] = {
    { 60, 110, 480 }, { 60, 110, 600 }, { 67, 110, 720 }, { 67, 110, 840 }, { 69, 110, 960 }, { 69, 110, 1080 },
    { 67, 230, 1200 }, { 65, 110, 1440 }, { 65, 110, 1560 }, { 64, 110, 1680 }, { 64, 110, 1800 },
    { 62, 110, 1920 }, { 62, 110, 2040 }, { 60, 210, 2160 }
};

// Data for "Twinkle twinkle", track 2  (BASS TRACK)
int track2_len = 10;
note_data track2[] = {
    { 48, 210, 480 }, { 48, 210, 720 }, { 53, 110, 960 }, { 53, 110, 1080 }, { 48, 210, 1200 }, { 50, 210, 1440 },
    { 48, 210, 1680 }, { 43, 110, 1920 }, { 43, 110, 2040 }, { 48, 210, 2160 }
};

// Data for "Twinkle twinkle", track 3  (DRUMS TRACK)
int track3_len = 47;
note_data track3[] = {
    { 35, 15, 480 }, { 42, 15, 540 }, { 38, 15, 600 }, { 42, 15, 600 }, { 35, 15, 660 }, { 42, 15, 660 },
    { 35, 15, 720 }, { 42, 15, 720}, { 42, 15, 780 }, { 38, 15, 840 }, { 42, 15, 840}, { 42, 15, 900 },
    { 35, 15, 960 }, { 42, 15, 1020 }, { 38, 15, 1080 }, { 42, 15, 1080 }, { 35, 15, 1140 }, { 42, 15, 1140},
    { 35, 15, 1200 }, { 42, 15, 1200}, { 42, 15, 1260 }, { 38, 15, 1320 }, { 42, 15, 1320 }, { 46, 15, 1380 },
    { 35, 15, 1440 }, { 42, 15, 1500 }, { 38, 15, 1560 }, { 42, 15, 1560 }, { 35, 15, 1620 }, { 42, 15, 1620 },
    { 35, 15, 1680 }, { 42, 15, 1680 }, { 42, 15, 1740 }, { 38, 15, 1800 }, { 42, 15, 1800 }, { 42, 15, 1860 },
    { 35, 15, 1920 }, { 42, 15, 1980 }, { 38, 15, 2040 }, { 42, 15, 2040 }, { 35, 15, 2100 }, { 42, 15, 2100 },
    { 35, 15, 2160 }, { 42, 15, 2160 }, { 42, 15, 2220 }, { 35, 15, 2280 }, { 49, 240, 2280 }
};
//////////////////////////////////////////////////////////////////
//                              M A I N                         //
//////////////////////////////////////////////////////////////////

int test_advancedsequencer_noinput( ) {
    ESP_LOGE(TAG,"Entering test_advancedsequencer_noinput example");
    ESP_LOGE(TAG,"************* DEBUG -4 *************");
    AdvancedSequencer sequencer; //was under GLOBALS, see above
    // gets the address of the sequencer MIDIMultiTrack, so we can edit it
   // sequencer.SetMIDIThruEnable(1);  //required for sending control messages 
	
    MIDIManager::OpenOutPorts(); //<<<<<<<<<<<<<<<< FCKX!! 220103
    
    //create a waiting loop for availability of port(s) NOT NECESSARY
    //it is integrated in the nimBLEdriver, that is called by Manager on instantiation of sequencer 
    //int concount = MIDIManager::GetOutDriver(0)->getConnectedCOunt();
    //int concount = MIDIManager::GetOutDriver(0)->IsPortOpen();
    ESP_LOGE(TAG,"WAITING A WHILE FOR SEQUENCER INITS (INCLUDING NIMBLE) TO FINISH"); 
    ESP_LOGE(TAG,"SHOULD BE DONE INSIDE THIS INIT, BY DETECTING PRESENCE OF CONNECTION WITH SYNTH");     
    MIDITimer::Wait(1000); //wait a while for a stable nimBLE connection
    ESP_LOGE(TAG,"NIMBLE CONNECTION SHOULD BE OK HERE"); 

    //while(true){   //get instatiation of tracks , trk , msg outside the loop
            ESP_LOGE(TAG,"************* DEBUG -3 *************");
    MIDIMultiTrack* tracks = sequencer.GetMultiTrack();
            ESP_LOGE(TAG,"************* DEBUG -2 *************");
    MIDITrack* trk;
    
    ESP_LOGE(TAG,"READY TO CREATE TRACK CONTENT"); 
    ESP_LOGE(TAG,"************* DEBUG -1 *************");
    // the constructor creates an undefined (NoOp) message with time 0
    MIDITimedMessage msg;
    int masterTrack = 0;
    int songTrack = 1;
    //use these
    //Reset 
    //Clear
    //GetNumEvents
    ESP_LOGE(TAG,"************* DEBUG 0 *************");
    //while(true){
    
    //MIDIManager::OpenOutPorts(); //<<<<<<<<<<<<<<<< FCKX!! 220103

    // now trk points to the master track (track 0 of the multitrack)
    trk = tracks->GetTrack(masterTrack);
    ESP_LOGE(TAG,"*************BEFORE CLEAN trk->GetNumEvents() masterTrack %d %d", masterTrack, trk->GetNumEvents());
    trk->Clear(); //clear to prevent multiple inserts when looping
    ESP_LOGE(TAG,"*************AFTER CLEAN trk->GetNumEvents() masterTrack %d %d", masterTrack, trk->GetNumEvents());


    msg.SetTimeSig(4, 4);           // remember that the message time is 0
    trk->InsertEvent(msg);          // inserts the time signature (4/4)
    msg.SetKeySig(0, 0);
    trk->InsertEvent(msg);          // inserts the key signature (CM)
    msg.SetTempo(200.0);
    trk->InsertEvent(msg);          // inserts the tempo
    
    
    ESP_LOGE(TAG,"************* DEBUG 1 *************");
    
    
    // now trk points to track 1, we'll use it for MIDI channel 1
    trk = tracks->GetTrack(songTrack);
    ESP_LOGE(TAG,"*************BEFORE CLEAN trk->GetNumEvents() songTrack %d %d", songTrack, trk->GetNumEvents());
    trk->Clear(); //clear to prevent multiple inserts when looping
    ESP_LOGE(TAG,"*************AFTER CLEAN trk->GetNumEvents() songTrack %d %d", songTrack, trk->GetNumEvents());

    // now trk points to track 1, we'll use it for MIDI channel 1
    trk = tracks->GetTrack(1);
    unsigned char channel = 0;      // MIDI channel 1 //Nic
     
    msg.SetProgramChange(channel, 128+0);
    trk->InsertEvent(msg);          // inserts the program change at time 0
    msg.SetVolumeChange(channel, 11);
    trk->InsertEvent(msg);          // inserts the track volume
    // and now inserts all the notes, taking them from our track1[] array
    ESP_LOGE(TAG,"************* DEBUG 2 *************");
    for(int i = 0; i < track1_len; i++) {
        msg.SetNoteOn(channel, track1[i].note, 100);
        msg.SetTime(track1[i].time);
        trk->InsertNote(msg, track1[i].length);
    }
    ESP_LOGE(TAG,"************* DEBUG 3 *************");
    // When you edit the AdvancedSequencer multitrack you must update the
    // sequencer parameters before playing: this does the job
    sequencer.SetMIDIThruEnable(true); //enable to pass thru control events
    sequencer.UpdateStatus();
    ESP_LOGE(TAG,"************* DEBUG 4 *************");
    
    
    // now we can play track 1 only
    cout << "Playing track 1 ..." << endl;
    sequencer.GoToZero();
    //sequencer.SetPlayMode(MIDISequencer::PLAY_BOUNDED);    
    //sequencer.SetRepeatPlay(1,3,0);   
    MIDITimer::SetResolution(1*portTICK_PERIOD_MS); //for ESP32 resolution must be a multiple of the system tick
    sequencer.Play();
    ESP_LOGE(TAG,"************* DEBUG 5 *************");
    while (sequencer.IsPlaying()) {
        MIDITimer::Wait(50); };
    ESP_LOGE(TAG,"************* DEBUG 6 *************");
    cout << "The sequencer finished" << endl;

// THE REST IS COMMENTED OUT. IF YOU SUCCEED CAN UNCOMMENT AND PLAY OTHER TWO TRACKS

#define BASS
#ifdef BASS
    // now do the same for track 2 (bass, channel 2)
    ESP_LOGE(TAG,"************* DEBUG 7 *************");
    trk = tracks->GetTrack(2);
    channel = 1;  //was 1
    ESP_LOGE(TAG,"************* DEBUG 8 *************");
    msg.Clear();                    // resets the message
    msg.SetProgramChange(channel, 128+15);
    trk->InsertEvent(msg);
    msg.SetVolumeChange(channel, 5);
    trk->InsertEvent(msg);
        ESP_LOGE(TAG,"************* DEBUG 9 *************");
    for(int i = 0; i < track2_len; i++) {
        msg.SetNoteOn(channel, track2[i].note, 100);
        msg.SetTime(track2[i].time);
        trk->InsertNote(msg, track2[i].length);
    }
    ESP_LOGE(TAG,"************* DEBUG 10 *************");
    sequencer.UpdateStatus(); //FCKX check check    
#endif //BASS


    //sequencer.GoToZero();


#define DRUMS
#ifdef DRUMS
    // ... and 3 (percussion, channel 10)
    ESP_LOGE(TAG,"************* DEBUG 11 *************");
    trk = tracks->GetTrack(3);
    channel = 9;
   
    msg.Clear();
    ESP_LOGE(TAG,"************* msg.SetProgramChange(channel, DRUMPRESET)  *************");
  //  msg.SetProgramChange(channel, DRUMPRESET); //was 33   
   msg.SetProgramChange(channel, 128+64+22);
  //uncomment these if your device doesn't sets
    trk->InsertEvent(msg);                // automatically the drums on channel 10
    msg.SetVolumeChange(channel, 8);
    trk->InsertEvent(msg);
    for(int i = 0; i < track3_len; i++) {
        msg.SetNoteOn(channel, track3[i].note, 100);
        msg.SetTime(track3[i].time);
        trk->InsertNote(msg, track3[i].length);
    }
    
    sequencer.UpdateStatus(); //FCKX check check 
#endif DRUMS
    

    //sequencer.GoToZero();
    ESP_LOGE(TAG,"************* DEBUG 12 *************");
#define PLAYSECOND
#ifdef PLAYSECOND
    ESP_LOGE(TAG,"************* DEBUG 13 *************");
    // while (true) {   
    cout << "Playing 3 tracks ..." << endl;
    sequencer.GoToZero();
    //SetRepeatPlay(1,3,0);        
    sequencer.Play();
    while (sequencer.IsPlaying())
        MIDITimer::Wait(50);
    cout << "    Stop Playing 3 tracks" << endl;
    //  } //while true
    
#endif //PLAYSECOND
    //sequencer.GoToZero();
    //MIDIManager::CloseOutPorts(); //<<<<<<<<<<<<<<<< FCKX!! 220103
    //
    //}//while true
    MIDIManager::CloseOutPorts(); //<<<<<<<<<<<<<<<< FCKX!! 220103
    ESP_LOGE(TAG,"Exiting test_advancedsequencer_noinput example");
    return EXIT_SUCCESS;
}


#endif // TEST_ADVANCEDSEQUENCER_NOINPUT


void app_main(void) {
/*************************************************************************************************
*NOTE: NimBLEData is a test object to demonstrate a globally accessible object 
*      It is instantiated in the header file with extern NimBLEGluer NimBLEData; 
*      More instantiations are UNWANTED. The opbject can be used immediately with e.g:
*      printf("MAIN testPointer: %d\n", NimBLEData.testPointer); //result: 123
*      NimBLEData.testPointer = 7;
*      printf("MAIN testPointer: %d\n", NimBLEData.testPointer); //result: 7
* 
*      The same method is used for nimBLEOutdriver that is an instantiation of class MidiOutNimBLE
*
*      The intention is that the object can be used in any translation unit (roughly speaking a .cpp)
*      WITHOUT another instatiation
*      Here we start with showing the first sign of life of this object:
****************************************************************************************************/
//Test
//ESP_LOGW(TAG,"nimBLEOutdriver getPortCount(): %d", (int)nimBLEOutdriver.getPortCount());
    static const char *TAG = "APP_MAIN"; 
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    esp_log_level_set("APP_MAIN", ESP_LOG_VERBOSE);
    ESP_LOGW(TAG, "APP_MAIN ENTRY"); 
//The next test is to see the server appear in a network sniffer (nRF Connect) 
//Until we implemented operation of the nimBLEOutdriver, the calls to NicMidi are commented  
#ifdef DEBUGON
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";
#endif
    
      
    ESP_LOGW(TAG, "APP_MAIN ENTRY");
//std::ostream& Utils::Log = MyStream1();  

//std::ostream& Log = MyStream1(); 
//std::ostream Log = MyStream1(); 

//3
/*
std::ostream::ostream() constructor is protected, which means it can only be invoked by its derived class, but not but its enclosing one.

To fix the error initialize member out  
https://stackoverflow.com/questions/14782809/issues-with-basic-ostream
https://stackoverflow.com/questions/14781629/proper-way-to-create-ostream-to-file-or-cout  !!!
https://stackoverflow.com/questions/366955/obtain-a-stdostream-either-from-stdcout-or-stdofstreamfile
*/

//std::ostream Log = MyStream2(std::cout.rdbuf()); //


    /*    
    ESP_LOGE - error (lowest)
    ESP_LOGW - warn
    ESP_LOGI - info
    ESP_LOGD - debug
    ESP_LOGV - verbose (highest)
    */    
    
    //look for existing "static const char *TAG" lines
    
    
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    esp_log_level_set("APP_MAIN", ESP_LOG_VERBOSE);
    esp_log_level_set("event", ESP_LOG_ERROR ); //MQTT
    esp_log_level_set("FCKX_SEQ", ESP_LOG_ERROR);
    esp_log_level_set("FCKX_SEQ_API", ESP_LOG_VERBOSE);
    esp_log_level_set("HANDLE_MIDI_SINGLE_IN", ESP_LOG_ERROR);
    esp_log_level_set("HANDLE_SEQ_COMMAND_IN", ESP_LOG_VERBOSE);
    
    
    esp_log_level_set("HARDWAREMSGIN", ESP_LOG_ERROR);
    esp_log_level_set("HANDLE_MIDI_SINGLE_IN",ESP_LOG_ERROR);
    esp_log_level_set("MIDIOutDriver", ESP_LOG_ERROR);
    esp_log_level_set("MIDIOutDriver::HardwareMsgOut", ESP_LOG_ERROR);
    esp_log_level_set("MIDIOutDriver::OutputMessage", ESP_LOG_ERROR);
    esp_log_level_set("MIDIOUTNIMBLE :: INITIALIZE", ESP_LOG_DEBUG);

    esp_log_level_set("MidiOutNimBLE :: sendMessage A", ESP_LOG_DEBUG);
    esp_log_level_set("MidiOutNimBLE :: sendMessage B", ESP_LOG_DEBUG);
    esp_log_level_set("MIDIThru::TickProc", ESP_LOG_DEBUG);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_ERROR);
    esp_log_level_set("mqtt_event_handler", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_event_handler_cb", ESP_LOG_VERBOSE);

    esp_log_level_set("NICMIDI HardwareMsgOut", ESP_LOG_ERROR);
    //esp_log_level_set("NICMIDI OutputMessage", ESP_LOG_VERBOSE);
    esp_log_level_set("NimBLE", ESP_LOG_VERBOSE);
    esp_log_level_set("NimBLEDevice.cpp", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_ERROR ); //MQTT
    
    esp_log_level_set("printMIDI_Input", ESP_LOG_VERBOSE);
    esp_log_level_set("RECORDER_FCKX", ESP_LOG_WARN);
    esp_log_level_set("SEQUENCER", ESP_LOG_ERROR);  

    
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    ESP_LOGW(TAG, "Initialize WIFI connection here");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
    * Read "Establishing Wi-Fi or Ethernet Connection" section in
    * examples/protocols/README.md for more information about this function.
    */
    //ESP_ERROR_CHECK(example_connect());   
       
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
   
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
   
    ESP_LOGW(TAG, "Initialize MQTT connection here");
    //esp_mqtt_client_handle_t  mqtt_client =  mqtt_app_start();
    mqtt_client =  mqtt_app_start();
    //esp_mqtt_client_handle_t  mqtt_client = 0; //to turn MQTT OFF
    
    //MidiOutNimBLE nimBLEOutdriver; //init nimBLEOut connection

    ESP_LOGE(TAG,"WAITING A WHILE FOR MQTT INITS TO FINISH");    
    MIDITimer::Wait(1000); 

   
    #ifdef TESTMESSAGE
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TESTMESSAGE");
    /**********************************************************************************
    *TEST  NiCMidi functionality
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/    
    ESP_LOGE(TAG,"Testing NiCMidi functionality: MidiMessage");    
    /*
    The MIDIMessage class allows you to create, edit or inspect MIDI messages without worrying about hexadecimal 
    values: it consists of a status byte, three data bytes for MIDI data, and a pointer to a MIDISystemExclusive 
    object (a buffer that can store any amount of bytes), with lots of methods for setting and inspecting data. 
    The constructor creates an empty (NoOp) message; you must then set its status and data with the class methods.    
    */

    MIDIMessage msg1, msg2, msg3;
    //MIDIMessage msg2;
    //MIDIMessage msg3;     // creates three empty MIDIMessage objects
    msg1.SetNoteOn(0, 60, 100);       // msg1 becomes a Note On, channel 1, note 60, velocity 100
    msg2.SetVolumeChange(0, 127);     // msg2 becomes a Volume Change (CC 7), channel 1, volume 127
    msg3.SetTimeSig(4, 4);            // msg 3 becomes a system Time Signature, 4/4
    msg1.SetChannel(msg1.GetChannel() + 1);  // increments the msg1 channel by one
    msg2.SetControllerValue(msg2.GetControllerValue() - 10);
    std::cout << msg1.MsgToText();    // prints a description of msg1
    printf("\n");
    std::cout << msg2.MsgToText();    // prints a description of msg2
    printf("\n");
    std::cout << msg3.MsgToText();    // prints a description of msg3
    printf("\n");


   printMsgBytes(msg1); //add an identifier
   printMsgBytes(msg2);
   printMsgBytes(msg3);

#endif //TESTMESSAGE


 
    /**********************************************************************************
    *END OF TEST  NiCMidi functionality
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/

#ifdef TEST_ADVANCEDSEQUENCER_NOINPUT
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_ADVANCEDSEQUENCER_NOINPUT");
 
 /*
 uint8_t chipid[6];esp_efuse_mac_get_custom(chipid);
    //esp_efuse_read_mac(chipid);
   
   // ESP_LOGI(TAG,"chipid %X",chipid[0]);
   ESP_LOGI(TAG,"chipid %s",chipid);
*/  


test_advancedsequencer_noinput(); //NiCMidi 211222

 ESP_LOGW(TAG,"END OF TEST_ADVANCEDSEQUENCER_NOINPUT"); 
/*
 while (true) {
 
//idle loop
vTaskDelay(500 / portTICK_PERIOD_MS);
 
};
*/    

#endif

#ifdef TEST_RECORDER_OLD
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER");
    ESP_LOGE(TAG,"instantiations for TEST_RECORDER EXAMPLE");
    ESP_LOGE(TAG,"Entering MIDISequencerGUINotifierText");
    MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier
    /*******************************************************************************************
    * AdvancedSequencer also instantiates nimBLEDevice (via sequencer, manager, driver)
    ********************************************************************************************/
    ESP_LOGE(TAG,"Entering AdvancedSequencer");
    AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)



    //some trials to learn about the MIDIManager interface and indrivers
    ESP_LOGE(TAG,"Learning about MIDIManager Interface"); 
    ESP_LOGE(TAG,"MIDIManager::GetNumMIDIIns() %d",MIDIManager::GetNumMIDIIns());          //FCKX
    ESP_LOGE(TAG,"MIDIManager::GetInDriver(0)->GetQueueSize() %d",MIDIManager::GetInDriver(0)->GetQueueSize()); 
    ESP_LOGE(TAG,"MIDIManager::GetInDriver(0)->CanGet() %d",MIDIManager::GetInDriver(0)->CanGet()); 
    //development helper to check analyze proper operation of the private data object        
    //MIDIManager::GetInDriver(0)->printData();


    ESP_LOGE(TAG,"Entering MIDIRecorder");                        
    MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX

    dumpflag = false;  //dumpflag is set when a message arrives over MQTT
                       //this leads activation of a DumpTrack in the continous loop
                       //after dumping the flag is reset, thus preventing dumping in each cycle
    ESP_LOGW(TAG, "START RECORDER test_recorder()");
    MIDIManager::AddMIDITick(&recorder);
    text_n.SetSequencer(&sequencer);
    
    #define RECORDLOOP
    #ifdef RECORDLOOP


    ESP_LOGW(TAG, "setEndRecTime 1");
    MIDIClockTime t = sequencer.MeasToMIDI(19,0); //endMeasure, endBeat   record the first 20 beats
    ESP_LOGW(TAG, "setEndRecTime 2");
    recorder.SetEndRecTime(t);

    ESP_LOGW(TAG, "setRepeatPlay 2");                                   

    //The following line causes a crash
    sequencer.SetRepeatPlay(true, 0, 10); //beginMeasure endMeasure      loop the first 11 beats

    #endif
 

 
 
    ESP_LOGW(TAG, "EnableTrack"); 
    recorder.EnableTrack(1);  //FCKX
    /*
        /// Sets the recording channel for the given track. This cannot be called during recording.
        /// \param trk_num the track number
        /// \param chan the channel: you can specify a number between 0 ... 15 or -1 for any channel.
        /// \return **true** if parameters are valid (and the channel has been changed), **false** otherwise.
        bool                            SetTrackRecChannel(unsigned int trk_num, char chan);
    */

    if (recorder.SetTrackRecChannel(1,0)) { // 
    recorder.Start(); }
    else {
       while(1) {
           ESP_LOGE(TAG,"SetTrackRecChannel FALSE");           
       }       
    }


    ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING test_recorder()");


    while (1) {      

    if (dumpflag) {
        ESP_LOGE(TAG, "********************* DUMP ALL Tracks **************************");    
        bool verbose = true;
       // DumpAllTracksAttr(sequencer.GetMultiTrack(), verbose);
        //DumpMIDITrack(sequencer.GetMultiTrack()->GetTrack(1));//
        if (recorder.GetMultiTrack()->IsValidTrackNumber(1)) {
                    MIDITrack* trk = recorder.GetTrack(1);
                    DumpMIDITrackWithPauses(trk, 1);              // in functions.cpp
                }
        dumpflag = false;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);    
    }  


#endif //ifdef TEST_RECORDER_OLD
  
#ifdef TEST_COMPONENT 
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_COMPONENT");
  ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING main_test_component()");
  while (1) {      
    main_test_component();  
    vTaskDelay(10 / portTICK_PERIOD_MS);  
  }
#endif

#ifdef TEST_RECORDING_NO_INPUT 
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDING_NO_INPUT");
  ESP_LOGW(TAG, "ENTERING test_recording_no_input()");
//  while (1) {      
    test_recording_no_input();  
        ESP_LOGE(TAG,"End of TEST_RECORDING_NO_INPUT CODE 2");  
    //vTaskDelay(10 / portTICK_PERIOD_MS);  
//  }
#endif


#ifdef TEST_RECORDER
ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER");
 //  ptrAdvancedSequencer = new AdvancedSequencer(&text_n);

 //  ptrMIDIRecorder = new MIDIRecorder(ptrAdvancedSequencer);

  ESP_LOGW(TAG, "ENTERING init_test_recorder()");
 // while (1) {      
        init_test_recorder();  
        ESP_LOGE(TAG,"End of TEST_RECORDER");  
    //vTaskDelay(10 / portTICK_PERIOD_MS);  
//  }
#endif //TEST_RECORDER




#ifdef TEST_THRU
    ESP_LOGE(TAG,"CONDITIONAL----------------------------------CONDITIONAL  TEST_THRU");
    ESP_LOGW(TAG, "GOINT TO INITALIZE TEST_THRU");
    int thru_result = test_thru();
    ESP_LOGW(TAG, "INITIALIZED TEST_THRU WITH RESULT: %d", thru_result);
    thru->Start(); // sets the MIDI thru on and off ORIG
    
    
//    MIDITimer::Wait(5000); 
//    thru->Stop();


    //delete thru;  //this code is never reached

#endif //ifdef TEST_THRU


/*
#else 
    //must create a LOOP!!!!!
    ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING N O T H I N G");
    while (1) {      
        //<your code here> 
        vTaskDelay(10 / portTICK_PERIOD_MS);  
    }
*/    
//#endif //#else
   

 ESP_LOGE(TAG,"Entering Idle Loop");
    while (1) {      
        //empty loop forever 
        ESP_LOGV(TAG,"Idle Loop");
        vTaskDelay(10 / portTICK_PERIOD_MS);  
    } 
  ESP_LOGE(TAG,"End of app_main");  
}; //app_main

