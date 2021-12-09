/***********************************************
* MQTTdriver for MIDI input , to be templated on:
*
*   - nimBLEdriver output port
*   - input driver code in RtMidi
*
* To be interfaced with manager.cpp and driver.cpp code (these codes have already been adapted. See MidiInMQTT class.
*/


#ifndef __MQTTDRIVER__
#define __MQTTDRIVER__

#include <string>   //maybe try to prevent using this
#include <vector>
//#include <NimBLEDevice.h>

/*******************
*MQTTdriver.h
********************/

/* 
NicMidi 211203:

I saw your driver.h file in the version 13 and I think that is the right way. You should implement a class with the same methods as the RtMidi ports and assign it the port attribute in the MIDIOutDriver class. NiCMidi uses only some methods of the RtMidi ports.
The implementation should be like this:


class MidiOutNimBLE {
    public:
                                     MidiOutNimBLE();
                                    ~MidiOutNimBLE();
                                
        void                         openPort(unsigned int portNumber=0);
        void                         closePort();
        virtual bool                 isPortOpen();
        unsigned int                 getPortCount()      { return 1; }
        std::string                  getPortName(unsigned int portNumber=0);
        void                         sendMessage(const std::vector<unsigned char> *message);
}
(see also the documentation of RtMidi)
Moreover you should modify the method MIDIManager::Init() (in the manager.h file) which attempts to open RtMidi ports, making it open your ports.
For the MIDIInDriver things are more complex as it relies on a callback called by RtMid

FCKX: make sure that the members of this class correpond to the state and state changes
that are expected by the equivalent class(es) in RtMidi and NicMidi

*/


struct MQTTMidiInData {
 /*   
    NimBLEServer* pServer;
    NimBLEService* pService; 
    NimBLECharacteristic* pCharacteristic;
    NimBLEAdvertising* pAdvertising;
*/
 //NimBLEServer* client;//this "client" is actually a nimBLE server
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



class MidiInMQTT   //:public MidiOutApi //(what does this parent class add?) 
{
    public:                          MidiInMQTT();
                                    //MidiOutMQTT(const std::string &clientName);
                                    ~MidiInMQTT();
                                
        void                         openPort(unsigned int portNumber=0);
        void                         closePort();
        //virtual bool                 isPortOpen();
        inline bool isPortOpen() const { return connected_; }
        unsigned int                 getPortCount();// { return 1; }
        std::string                  getPortName(unsigned int portNumber=0);
        void                         sendMessage(const std::vector<unsigned char> *message);
        //setcallback1               see rtMidi for examples
        //setcallback2
        
    protected:
        MQTTMidiInData connectionData;
        void initialize( const std::string& clientName  ); 
        void *apiData_;   //in the RtMidi case this is in class .... MidiApi
        bool connected_;  //in the RtMidi case this is in class .... MidiApi      
        //callback1  ee rtMidi for examples
        //callback2

};


#endif