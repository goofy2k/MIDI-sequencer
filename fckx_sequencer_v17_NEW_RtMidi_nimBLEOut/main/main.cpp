//CONSIDER TO RENAME BLEDevice etc to NimBLEDevice etc. But test carefully when you have running code!
//tmove references to the old jdks lib
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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

#include "nimBLEdriver.h" //make driver globally accessible by including this header file



//#define NIMBLE_IN_MAIN            //when is it considered as defined? what value is needed?

//switches for NiCMidi examples in order of priority for implementation
//https://ncassetta.github.io/NiCMidi/docs/html/examples.html
#define MIDIMESSAGE                //not an example but inspired by web page <ref> OK in main
#define MIDITRACK_DUMPMIDITRACK    //OK in main
//metronome //output only
//thru //input and output

//the following (adapted) examples depend on nimBLEdriver bluetooth output by FCKX
//use only one at a time as instantiation of output ports may interfere (solve this in the future for flxibility)

//#define TEST_COMPONENT           //test_component example OK (in main eternal loop) 
#define TEST_RECORDER            //test_recorder
//#define TEST_ADVANCEDSEQUENCER     //test_advancedsequencer



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

#ifdef NIMBLE_IN_MAIN   //phase out
    #include <NimBLEDevice.h>
#endif
//#include "fckxMsg.h"
#include "queue.h"   //MUST BE ON

//NiCMidi includes
//REMOVED AS YOU NOW USE IT AS A COMPONENT
//#include "../include/msg.h" 
//#include "../include/sysex.h"
//#include "../include/midi.h"
//#include "../include/track_dirty.h"



#include "msg.h"
#include <iostream>
#include "track.h"
#include "dump_tracks.h"
#include "sysex.h"
#include "../include/tick.h"       //check if you can harmonize these paths FCKX
#include "../include/manager.h"

//define queue sizes
//max total queue size is at least 2048 (fckx_sequencer_v4)
#define INQ_SIZE    128
#define OUTQ_SIZE   128
#define SEQ_SIZE    1024

//#include "jdksmidi/track.h" //FCKX
//#include <jdksmidi/track.h> //FCKX
//#include "track.h" //FCKX
//#include <track.h> //FCKX

extern "C" {void app_main(void);}

#ifdef NIMBLE_IN_MAIN
//test using "NIM" before "BLE"
BLEServer* pServer = NULL; //FCKX phase out //compile conditionally
BLECharacteristic* pCharacteristic = NULL;  //FCKX phase out
#endif


static const char *TAG = "FCKX_SEQ";

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

static void event_handler(void* arg, esp_event_base_t event_base,
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
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
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
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
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


/*
void execute_single_midi_command(DspFaust * aDSP, int mididata){  //uses propagateMidi
static const char *TAG = "execute_single_midi_command";
      //unsigned long eventTimestamp = generate_Timestamp(xTaskGetTickCount());
    unsigned long event_Timestamp =  generate_Timestamp( xTaskGetTickCount() ) ;
    jdksmidi::MIDITimedBigMessage inMessage = generate_MIDITimedBigMessage( mididata, event_Timestamp);
    
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
        
 };
*/

#ifdef NIMBLE_IN_MAIN  //phase out

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





 //may extend to class

jdksmidi::MIDIQueue inQ(INQ_SIZE); 
int bufCount = 0;  

 

namespace jdksmidi {
    
 

bool insertMIDI_Sorted(MIDIQueue inQ, MIDIQueue sortedQueue){
    bool result = false;
    //insert the items that are in the (unsorted) inQ into a position in the sortedBuffer
    
    
   return result; 
};

}


bool storeMIDI_Input(esp_mqtt_event_handle_t event){

//bool storeMIDI_Input(esp_mqtt_event_handle_t event, jdksmidi::MIDIQueue inQ){
    // store incoming MQTT MIDI event in input buffer    
    static const char *TAG = "storeMIDI_Input"; 
    bool result = false;   
     //display incoming data
    ESP_LOGI(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
    ESP_LOGI(TAG,"DATA:%.*s ", event->data_len, event->data);
    ESP_LOGI(TAG,"DATA0 %d", event->data[0]);
    ESP_LOGI(TAG,"DATA1 %d", event->data[1]); 
    ESP_LOGI(TAG,"DATA2 %d", event->data[2]); 
    ESP_LOGI(TAG,"DATA3 %d", event->data[3]); 
    ESP_LOGI(TAG,"DATA4 %d", event->data[4]);
    
    
    //SEQUENCER TASK 3.
    //prepare for storing MIDI input
    
    fckxMIDITimedMessage inMsg;
    inMsg.timestamp = xTaskGetTickCount();
    inMsg.status = event->data[2];    
    inMsg.data1 = event->data[3];  
    inMsg.data2 = event->data[4];  
   
    ESP_LOGI(TAG,"inMsg.timestamp %lu", inMsg.timestamp);
    ESP_LOGI(TAG,"inMsg.status %d", inMsg.status); 
    ESP_LOGI(TAG,"inMsg.data1 %d", inMsg.data1); 
    ESP_LOGI(TAG,"inMsg.data2 %d", inMsg.data2); 
     //printf("storeMIDI_Input");
     printf("bufCount %d\n",bufCount);
     if (inQ.CanGet()) {
         printf("CanGet true\n");
     //ESP_LOGE(TAG,"inQ.CanGet() is true"); 
     } else {
     ESP_LOGE(TAG,"inQ.CanGet() is false"); }
     
    if (inQ.CanPut()) {
       // ESP_LOGE(TAG,"inQ.CanPut() is true");
       printf("CanPut true\n");
         inQ.Put(inMsg); 
        
    } else {
       ESP_LOGE(TAG,"inQ.CanPut() is false");
       //printf("CanPut false");
    };
   bufCount = bufCount + 1;
   



    //Append inMsg to input buffer    
    //<code here>    
    
    return result;
};


//#ifdef NIMBLE_IN_MAIN   this is only because of midi thru!!!!!  so switch off thru

static void call_fckx_seq_api(esp_mqtt_event_handle_t event){

    //receive a 5 byte array 
 
    static const char *TAG = "FCKX_SEQ_API";
    
    //MIDI COMMANDS               
    if (strncmp(event->topic, "/fckx_seq/midi/single",strlen("/fckx_seq/midi/single")) == 0) {
    //receive a 5 byte MIDI message 

    //display incoming MIDI data
    ESP_LOGI(TAG,"COMMAND:%.*s ", event->topic_len, event->topic); 
    ESP_LOGI(TAG,"DATA:%.*s ", event->data_len, event->data);
    ESP_LOGI(TAG,"DATA0 %d", event->data[0]);
    ESP_LOGI(TAG,"DATA1 %d", event->data[1]); 
    ESP_LOGI(TAG,"DATA2 %d", event->data[2]); 
    ESP_LOGI(TAG,"DATA3 %d", event->data[3]); 
    ESP_LOGI(TAG,"DATA4 %d", event->data[4]);
    
    //prepare for immediate sending to sound board (MIDI THRU)
    //create midiPacket for BLE notify
    uint8_t midiPacket[] = {
    event->data[0], //header
    event->data[1], //timestamp, not implemented
    event->data[2], //status
    event->data[3], //== 60 == middle c
    event->data[4] //velocity
        };//
        
       //store in input buffer
   //storeMIDI_Input(event, inQ );
   printf("going to store input\n");
storeMIDI_Input(event);    
        
    //if MIDI THRU
    //send midiPacket immediately to sound board
    //add check on NimBLE connection
    
    
    //switch of thru
    //pCharacteristic->setValue(midiPacket, 5);
   // pCharacteristic->notify();
          } 
        
    else
    //NONE MIDI COMMANDS (e.g. for NiCMidi MIDIManager        
    if (strncmp(event->topic, "/fckx_seq/command",strlen("/fckx_seq/command")) == 0) {
        ESP_LOGI(TAG,"COMMAND:%.*s\r ", event->topic_len, event->topic);
        ESP_LOGI(TAG,"...command to be implemented...");      
        } 
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
            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq/midi/single",0);
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
#include "../examples/functions.h"                  // helper functions for input parsing and output

//for test_recorder
#include "../include/advancedsequencer.h"
#include "../include/recorder.h"
#include "../include/filewritemultitrack.h"


using namespace std;

//////////////////////////////////////////////////////////////////
//                 TEST_RECORDER G L O B A L S                  //
//////////////////////////////////////////////////////////////////

/*
        RtMidiOut temp_MIDI_out;
        for (unsigned int i = 0; i < temp_MIDI_out.getPortCount(); i++) {
            MIDI_outs->push_back(new MIDIOutDriver(i));
            MIDI_out_names->push_back(temp_MIDI_out.getPortName(i));
*/            
            

//AdvancedSequencer sequencer;       // an AdvancedSequencer (without GUI notifier)
//RtMidi temp_rtMIDI;

//for (unsigned int i = 0; i < temp_MIDI_out.getPortCount(); i++) { printf("SUCCESS!\n")}
//RtMidi temp_RtMidi;
//RtMidi::temp_RtMidi.add_Nimble(); //HAVE TO DELETE TEMP MIDI OUT AFTER TRANSFERRING POINTER
//temp_RtMidi.add_Nimble(); //HAVE TO DELETE TEMP MIDI OUT AFTER TRANSFERRING POINTER

//RtMidi::add_NimBLE();  //helper to pass pointer at nimBLE server to RtMidi

//class that holds nimBLE setup data after server has been set up
//the MidOutNimBLE::intitialize must have access to a function that is a friend  of nimBLEDATA




#ifdef TEST_ADVANCEDSEQUENCER


MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier

/*******************************************************************************************
* AdvancedSequencer also instantiates nimBLEDevice (via sequencer, manager, driver, RtMidi)
********************************************************************************************/
AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)
                        
MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX
#endif
//extern string command, par1, par2;          // used by GetCommand() for parsing the user input
//char filename[200];                         // used for saving files
/*
const char helpstring[] =                   // shown by the help command
"\nAvailable commands:\n\
   load filename          : Loads the file into the sequencer; you must specify\n\
                          : the file name with .mid extension\n\
   save filename          : Save the current multitrack into a file; If the\n\
                          : file name is not specified you must type it with\n\
                          : .mid extension)\n\
   ports                  : Enumerates MIDI In and OUT ports\n\
   play                   : Starts playback from current time\n\
   stop                   : Stops playback\n\
   rec on/off             : Enable/disable recording\n\
   addtrack [n]           : Insert a new track (if n is not given appends\n\
                            it)\n\
   deltrack [n]           : Deletes a track (if n is not given deletes the\n\
                            last track)\n\
   enable trk [chan]      : Enable track trk for recording (if you don't specify\n\
                            the channel this will be the track channel if the\n\
                            track is a channel track or all channels\n\
   disable [trk]          : Disable track trk for recording (if you omit trk disables\n\
                          : all tracks)\n\
   recstart [meas] [beat] : Sets the recording start time from meas and beat\n\
                          : (numbered from 0) If you don't specify anything from 0.\n\
   recend[meas] [beat]    : Sets the recording end time to meas and beat. If you\n\
                            don't specify anything to infinite.\n\
   undo                   : Restore the sequence content before the recording.\n\
                            You have multiple undo levels.\n\
   rew                    : Goes to the beginning (stops the playback)\n\
   goto meas [beat]       : Moves current time to given meas and beat\n\
                            (numbered from 0)\n\
   dumps [trk]            : Prints a dump of all midi events in the sequencer\n\
                            (or in its track trk)\n\
   dumpr [trk]            : Prints a dump of all midi events in the recorder\n\
                            (or in its track trk)\n\
   inport track port      : Sets the MIDI in port for the given track\n\
   outport track port     : Sets the MIDI out port for the given track\n\
   program track p        : Sets the MIDI program (patch) for the given track\n\
   volume track v         : Sets the MIDI volume for the given track\n\
   trackinfo [v]          : Shows info about all tracks of the file. If you\n\
                            add the v the info are more complete.\n\
   b                      : (backward) Moves current time to the previous measure\n\
   f                      : (forward) Moves current time to the next measure\n\
   help                   : Prints this help screen\n\
   quit                   : Exits\n\
The recording related commands can be given only when the sequencer is stopped,\n\
other commands even during playback\n";
*/

/*
int main_test_midiports() {
    if (MIDIManager::GetNumMIDIIns()) {
        cout << "MIDI IN PORTS:" << endl;
        for (unsigned int i = 0; i < MIDIManager::GetNumMIDIIns(); i++)
            cout <<'\t' << i << ": " << MIDIManager::GetMIDIInName(i) << endl;
    }
    else
        cout << "NO MIDI IN PORTS" << endl;
    if (MIDIManager::GetNumMIDIOuts()) {
        cout << "MIDI OUT PORTS:" << endl;
        for (unsigned int i = 0; i < MIDIManager::GetNumMIDIOuts(); i++)
            cout << '\t' << i << ": " << MIDIManager::GetMIDIOutName(i) << endl;
    }
    else
        cout << "NO MIDI OUT PORTS" << endl;
    cout << endl << endl << "Press ENTER" << endl;
    cin.get();
    return EXIT_SUCCESS;
}
*/

/*
Metronome metro;                                // a Metronome (without GUI notifier)

//extern string command, par1, par2;              // used by GetCommand() for parsing the user input
//for commands: send a jsonStr that can be converted into an object with string command, and array of string par
//see Nodered
//to prevent the use of json decoding: call the api with:  /fckx_seq/command/<command_name>/<par1_value>,<par2_value
//or implement: JSON::Parse(json_string)
//build a GUI similar to the one for the audio decoder in the sound board firmware.
//where is the data, the status of the sequencer....?
//for the data: see the test_win32_player.cpp example and refactor it to JSONUI for a browser based GUI
//be very very hesitant to use the Windows API

const char helpstring[] =                       // shown by the help command
"\nAvailable commands:\n\
   ports               : Enumerates MIDI In and OUT ports\n\
   start               : Starts the metronome\n\
   stop                : Stops the metronome\n\
   tempo bpm           : Sets the metronome tempo (bpm is a float)\n\
   tscale scale        : Sets global tempo scale. scale is in percent\n\
                         (ex. 200 = twice faster, 50 = twice slower)\n\
   subd n              : Sets the number of subdivisions (n can be\n\
                         0, 2, 3, 4, 5, 6, 0 disables subdivisions)\n\
   meas n              : Sets the number of beats of a measure (0 disables\n\
                         measure clicks)\n\
   measnote nn         : Sets the MIDI note for first beat of the measure\n\
   beatnote nn         : Sets the MIDI note for ordinary beats\n\
   subdnote nn         : Sets the MIDI note for subdivisions\n\
   outport port        : Sets the MIDI out port\n\
   outchan ch          : Sets the MIDI out channel\n\
   status              : Prints the status of the metronome\n\
   help                : Prints this help screen\n\
   quit                : Exits\n\
MIDI channels are numbered 0 .. 15\n\
All commands can be given during playback\n";


//////////////////////////////////////////////////////////////////
//                              M A I N                         //
//////////////////////////////////////////////////////////////////

int main_test_metronome( string command) {
//int main_test_metronome( int argc, char **argv ) {
    // tests if a MIDI out port is present in the system
    if (MIDIManager::GetNumMIDIOuts() == 0) {
        cout << "This program has no functionality without MIDI out ports!\n" <<
                "Press a key to quit\n";
        cin.get();
        return EXIT_SUCCESS;
    }
    // adds the metronome to the MIDIManager queue
    MIDIManager::AddMIDITick(&metro);

    cout << "TYPE help TO GET A LIST OF AVAILABLE COMMANDS\n\n";
   std::string str(command); 
  // while ( strcmp(command, "quit") != 0 )  // main loop
  while (command != "quit") {                     // main loop
        //GetCommand();                               // gets user input and splits it into command, par1, par2
        //replace this by parsing a json object, from a jsonStr sent by Nodered
        
        if(command == "")                           // empty command
            continue;
        if (command == "ports") {                   // enumerates the midi ports
            if (MIDIManager::GetNumMIDIIns()) {
                cout << "MIDI IN PORTS:" << endl;
                for (unsigned int i = 0; i < MIDIManager::GetNumMIDIIns(); i++)
                    cout << i << ": " << MIDIManager::GetMIDIInName( i ) << endl;
            }
            else
                cout << "NO MIDI IN PORTS" << endl;
            cout << "MIDI OUT PORTS:" << endl;
            for (unsigned int i = 0; i < MIDIManager::GetNumMIDIOuts(); i++)
                cout << i << ": " << MIDIManager::GetMIDIOutName( i ) << endl;
        }
        else if (command == "start") {               // starts the metronome
            metro.Start();
            cout << "Metronome started" << endl;
        }
        else if (command == "stop") {               // stops the metronome
            metro.Stop();
            cout << "Metronome stopped at " << metro.GetCurrentMeasure() << ":"
                 << metro.GetCurrentBeat() << endl;
        }
        else if (command == "tempo") {              // changes the metronome tempo
            float tempo = atof(par1.c_str());
            if (metro.SetTempo(tempo)) {
                cout << "Tempo set to " << tempo <<
                        "  Effective tempo: " << metro.GetTempoWithScale() << " bpm" << endl;
            }
            else
                cout << "Invalid tempo" << endl;
        }
        else if (command == "tscale") {             // scales the metronome tempo
            int scale = atoi(par1.c_str());
            if (metro.SetTempoScale(scale)) {
                cout << "Tempo scale : " << scale <<
                        "%  Effective tempo: " << metro.GetTempoWithScale() << " bpm" << endl;
            }
            else
                cout << "Invalid tempo scale" << endl;
        }
        else if (command == "subd") {               // sets the number of subdivision of each beat
            unsigned int type = atoi(par1.c_str());          // 0 disables subdivision clicks
            if (metro.SetSubdType(type)) {
                if (type == 0)
                    cout << "Subdivision click disabled" << endl;
                else
                     cout << "Number of subdivisions set to " << type << endl;
            }
            else
                cout << "Invalid number of subdivisions" << endl;
        }
        else if (command == "meas") {               // sets the number of beats of a measure
            unsigned int beats = atoi(par1.c_str());         // 0 disables measure clicks
            metro.SetTimeSigNumerator(beats);
            if (beats == 0)
                cout << "First beat disabled" << endl;
            else
                cout << "Beats set to " << beats << endl;
        }

        else if (command == "measnote") {           // sets the note for 1st beat of a measure
            unsigned int note = atoi(par1.c_str()) & 0x7f;
            metro.SetMeasNote(note);
            cout << "First beat note set to " << note << endl;
        }
        else if (command == "beatnote") {           // sets the note for ordinary beats
            unsigned int note = atoi(par1.c_str()) & 0x7f;
            metro.SetBeatNote(note);
            cout << "Beat note set to " << note << endl;
        }
        else if (command == "subdnote") {           // sets the note for subdivisions
            unsigned int note = atoi(par1.c_str()) & 0x7f;
            metro.SetSubdNote(note);
            cout << "Subdivision note set to " << note << endl;
        }
        else if (command == "outport") {            // changes the midi out port
            int port = atoi(par1.c_str());
            if (metro.SetOutPort(port))
                cout << "Assigned out port n. " << port << endl;
            else
                cout << "Invalid port number" << endl;
        }
        else if (command == "outchan") {            // changes the midi out chan
            int chan = atoi(par1.c_str());
            if (metro.SetOutChannel(chan))
                cout << "Assigned out channel n. " << (int)chan << endl;
            else
                cout << "Invalid channel number" << endl;
        }
        else if (command == "status") {
            cout << "\nMETRONOME STATUS:\n";
            cout << "MIDI out port:             " << MIDIManager::GetMIDIOutName(metro.GetOutPort()) << endl;
            cout << "MIDI out channel:          " << int(metro.GetOutChannel()) << endl;
            cout << "Tempo:                     " << metro.GetTempoWithoutScale() << " bpm" << endl;
            cout << "Tempo scale:               " << metro.GetTempoScale() << "% (effective tempo: " <<
                    metro.GetTempoWithScale() << " bpm)" << endl;
            cout << "Measure beats:             " << int(metro.GetTimeSigNumerator());
            cout << (metro.GetTimeSigNumerator() == 0 ? " (disabled)" : "") << endl;
            cout << "Subdivision:               " << int(metro.GetSubdType());
            cout << (metro.GetSubdType() == 0 ? " (disabled)" : "") << endl;
            cout << "Measure beat note:         " << int(metro.GetMeasNote()) << endl;
            cout << "Ordinary beat note:        " << int(metro.GetBeatNote()) << endl;
            cout << "Subdivision note:          " << int(metro.GetSubdNote()) << endl;
        }
        else if (command == "help")                 // prints help screen
            cout << helpstring;
        else if (command != "quit" )                // unrecognized command
            cout << "Unrecognized command" << endl;
    }
    return EXIT_SUCCESS;
}

*/
// If you want to implement your own MIDITickComponent derived class you must at least redefine
// the StaticTickProc() and TickProc() methods (and probably Start() and Stop() also).
// Before using the class you must add it to the MIDIManager queue with the
// MIDIManager::AddMIDITick().
// This is a very simple example which play a fixed note every second; see the comments
// to every method for details.
//



    /**********************************************************************************
    *TEST  NiCMidi functionality   test_component.cpp
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/ 

#ifdef TEST_COMPONENT  

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
/*
    if (deltat >= next_note_off) {              // we must turn off the note
        msg.SetNoteOff(0, 36, 0);
        //MIDIManager::GetOutDriver(0)->OutputMessage(msg);
                                                // sends a note off message to the MIDI 0 port
        sendToMIDIOut(msg);                         // sends a note off message to the BLE interface               
        cout << "and off" << endl;
        next_note_off += NOTE_INTERVAL;         // updates the next note off time
    }
*/
    if (deltat >= next_note_on) {               // we must turn on the note
        msg.SetNoteOn(0, 36, 127);
        MIDIManager::GetOutDriver(0)->OutputMessage(msg);  //phase in
                                                // sends a note on message to the MIDI 0 port
    //    sendToMIDIOut(msg);            //phase out             // sends a note off message to the BLE interface                                                                                                                                                                                      
        cout << "Note on ... ";
        next_note_on += NOTE_INTERVAL;          // updates the next note on time
    }
     //third event added by FCKX
    if (deltat >= next_note_off) {              // we must turn off the note
        msg.SetNoteOff(0, 36, 0);
        MIDIManager::GetOutDriver(0)->OutputMessage(msg);
                                                // sends a note off message to the MIDI 0 port
        //sendToMIDIOut(msg);                   // sends a note off message to the BLE interface               
        cout << "...  and off" << endl;
        next_note_off += NOTE_INTERVAL;         // updates the next note off time
    }
    
}

// The main() creates a class instance, adds it to the MIDIManager queue and then calls
// Start() and Stop() for enabling and disabling the callback
int main_test_component() {
    TestComp comp;                              // creates the component
    MIDIManager::AddMIDITick(&comp);            // adds it to the MIDIManager queue
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


//The next test is to see the server appear in a network sniffer (nRF Connect) 
//Until we implemented operation of the nimBLEOutdriver, the calls to NicMidi are commented  

    
    static const char *TAG = "APP_MAIN";   
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("NimBLE", ESP_LOG_VERBOSE);
    esp_log_level_set("storeMIDI_Input", ESP_LOG_INFO);
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    ESP_LOGW(TAG, "Initialize WIFI connection here");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
//jdksmidi::MIDIQueue inQ(5);  
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
   esp_mqtt_client_handle_t  mqtt_client =  mqtt_app_start();
  //esp_mqtt_client_handle_t  mqtt_client = 0; //to turn MQTT OFF
   //MidiOutNimBLE nimBLEOutdriver; //init nimBLEOut connection

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

    /*
    printf("msg1.MsgToText(): %s\n", msg1.MsgToText()); //does not print
    printf("msg2.MsgToText(): %s\n", msg2.MsgToText()); 
    printf("msg3.MsgToText(): %s\n", msg3.MsgToText()); 
    */ 

//Can I use the NicMidi Track class?
/*
The constructor creates an empty track, with only the EOT; you can then edit it: the MIDITrack::InsertEvent(), MIDITrack::InsertNote(), MIDITrack::DeleteEvent() and MIDITrack::DeleteNote() methods insert and delete MIDITimedMessage objects, while other methods can insert or delete entire time intervals.

Here is an example:
*/
//#include "track.h"
//#include "dump_tracks.h"             // contains helper functions to print track content
//#include "../include/dump_tracks_dirty.h"   //FCKX          // contains helper functions to print track content
   ESP_LOGE(TAG,"Testing NiCMidi functionality: MidiTrack/DumpMidiTrack");
//int main() {
   MIDITrack track;
   MIDITimedMessage msg;             // a new MIDITimedMessage has time set to 0
   msg.SetProgramChange(0, 49);      // msg becomes a Program Change, channel 1, program 49, time 0
   track.InsertEvent(msg);           // inserts a copy of msg into the track
   msg.SetVolumeChange(0, 127);      // msg becomes a Volume Change (CC 7), channel 1, volume 127, time 0
   track.InsertEvent(msg);
   msg.SetNoteOn(0, 60, 100);        // msg becomes a Note On, channel 1, note 60, velocity 100
//   msg3.SetTime(480);                // sets the time of msg to 480 MIDI ticks
   track.InsertNote(msg, 240);       // inserts the Note On and the corresponding Note Off after 240 ticks
   DumpMIDITrack(&track);            // prints the contents of the track
//   return 0;
//}

/*  //FCKX
  ESP_LOGE(TAG,"Testing NiCMidi functionality: MIDItimer MIDITickComponent, MIDIManager");
  ESP_LOGE(TAG,"Testing NiCMidi functionality: test_component.cpp");    
  main(); //code above
*/  
 
    /**********************************************************************************
    *END OF TEST  NiCMidi functionality
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/
  
  //code for NicMidi test_recorder example
  //MIDIManager::AddMIDITick(&recorder);
  //text_n.SetSequencer(&sequencer);
   
  //NimBLE Bluetooth
  //Create the BLE Device

#ifdef TEST_RECORDER
    ESP_LOGE(TAG,"instantiations for TEST_RECORDER EXAMPLE");
    ESP_LOGE(TAG,"Entering MIDISequencerGUINotifierText");
    MIDISequencerGUINotifierText text_n;        // the AdvancedSequencer GUI notifier
    /*******************************************************************************************
    * AdvancedSequencer also instantiates nimBLEDevice (via sequencer, manager, driver, RtMidi)
    ********************************************************************************************/
    ESP_LOGE(TAG,"Entering AdvancedSequencer");
    AdvancedSequencer sequencer(&text_n);       // an AdvancedSequencer (with GUI notifier)
    ESP_LOGE(TAG,"Entering MIDIRecorder");                        
    MIDIRecorder recorder(&sequencer);          // a MIDIRecorder //FCKX
#endif //ifdef TEST_RECORDER 
  

  
#ifdef TEST_COMPONENT 
  ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING main_test_component()");
  while (1) {      
    main_test_component();  
    vTaskDelay(10 / portTICK_PERIOD_MS);  
  }
 
#else //TEST_RECORDER
   ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING test_recorder()");
  while (1) {      
    //code here 
    bool verbose = true;
    DumpAllTracksAttr(sequencer.GetMultiTrack(), verbose);
    vTaskDelay(10 / portTICK_PERIOD_MS);  
  } 
  

/*
#else 
    //must create a LOOP!!!!!
    ESP_LOGW(TAG, "ENTERING MAIN LOOP EXECUTING N O T H I N G");
    while (1) {      
        //<your code here> 
        vTaskDelay(10 / portTICK_PERIOD_MS);  
    }
*/    
#endif //#else
    

}; //app_main

