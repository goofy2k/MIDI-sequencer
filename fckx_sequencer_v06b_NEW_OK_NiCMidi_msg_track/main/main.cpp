

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

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

#include "secrets.h"

//Credentials form secrets.h
#define EXAMPLE_ESP_WIFI_SSID      SECRET_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      SECRET_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  SECRET_ESP_MAXIMUM_RETRY

#include <NimBLEDevice.h>
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

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;



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

void notify_midi(BLECharacteristic* pCharacteristic, unsigned long int mididata){
 

//MIDI data packet, taken from Brian Duhan arduino-esp32-BLE-MIDI / BLE_MIDI.ino
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


void fckx_single_midi_command(unsigned long int mididata){  //uses propagateMidi
static const char *TAG = "execute_single_midi_command";
      //unsigned long eventTimestamp = generate_Timestamp(xTaskGetTickCount());
    //unsigned long event_Timestamp =  generate_Timestamp( xTaskGetTickCount() ) ;
    //jdksmidi::MIDITimedBigMessage inMessage = generate_MIDITimedBigMessage( mididata, event_Timestamp);
  notify_midi(pCharacteristic, mididata);
  
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


bool handle_inQ(){
    //look in input buffer
    bool result = false;
    
    
    return result;
};


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




static void call_fckx_seq_api(esp_mqtt_event_handle_t event){

    //receive a 5 byte array 
 
    static const char *TAG = "FCKX_SEQ_API";
    
    if (strncmp(event->topic, "/fckx_seq/midi/single",strlen("/fckx_seq/midi/single")) == 0) {


    //display incoming data
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
    pCharacteristic->setValue(midiPacket, 5);
    pCharacteristic->notify();
    
 
    
        } 
    else {
         ESP_LOGI(TAG,"COMMAND:%.*s\r ", event->topic_len, event->topic);
        ESP_LOGI(TAG,"unknown API command");}
};
    

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
            
            msg_id = esp_mqtt_client_subscribe(client, "/fckx_seq", 0);
            //ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_publish(client, "/fckx_seq", "MQTT OK", 0, 1, 0);
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

                if (strncmp(event->topic, "/fckx_seq/",strlen("/fckx_seq/")) == 0) { //this measn: string starts with ....
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

void connectedTask (void * parameter){
    for(;;) {  //loop forever
        // only act if connection status changes or if there is a stable connection
        if (deviceConnected) {
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
            ESP_LOGW(TAG,"Start advertising again");   
            pServer->startAdvertising();        // restart advertising
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


    
void app_main(void) {
    
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




  
  
    /**********************************************************************************
    *END OF TEST  NiCMidi functionality
    *
    * https://ncassetta.github.io/NiCMidi/docs/html/_m_e_s_s__t_r_a_c_k__m_u_l_t_i.html
    *    
    ***********************************************************************************/
  
  
  
  
  

  
  // Create the BLE Device
  ESP_LOGI(TAG, "Initialize BLEDevice fckx_seq");
  BLEDevice::init("fckx_seq");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

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

  // Start advertising
  ESP_LOGI(TAG, "Start advertising");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  /** This method had been removed **
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  **/
  //define a task with a continous loop
  //it is called connectedTask, but in fact it also starts when there is no connection
  xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);
  
  BLEDevice::startAdvertising();
  ESP_LOGI(TAG, "Waiting for a client connection to notify...");
  //printf("Waiting a client connection to notify...\n");
}
