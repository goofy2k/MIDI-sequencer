/***********************************************
*definitions to make NimBLE driver varaiables globally accessible
*see: https://stackoverflow.com/questions/19929681/c-global-variable-declaration/19929727
*answer by Wolf, but learn more!
*/


#ifndef __NIMBLEDRIVER__
#define __NIMBLEDRIVER__

#include <string>   //maybe try to prevent using this
#include <vector>

//extern int globaltestPointer = 456; //extern to make it globally accessible????

/*******************
*nimBLEdriver.h
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

class MidiOutNimBLE   //:public MidiOutApi //(what does this parent class add?) 
{
    public:                          MidiOutNimBLE();
                                    //MidiOutNimBLE(const std::string &clientName);
                                    ~MidiOutNimBLE();
                                
        void                         openPort(unsigned int portNumber=0);
        void                         closePort();
        //virtual bool                 isPortOpen();
        unsigned int                 getPortCount();// { return 1; }
        std::string                  getPortName(unsigned int portNumber=0);
        void                         sendMessage(const std::vector<unsigned char> *message);

    protected:
        void initialize( const std::string& clientName  ); 
        void *apiData_;   //in the RtMidi case this is in class .... MidiApi
        bool connected_;  //in the RtMidi case this is in class .... MidiApi      
};

extern MidiOutNimBLE nimBLEOutdriver; //to make it globally accessible as demonstrated with NimBLEGluer (see above)
                                      //is this really necessary?



//Class to test passing data between two translation units (.cpp's)

class NimBLEGluer {
    
    public:
    //constructor
    //NimBLEGluer(); //causes a: ../main/main.cpp:564: undefined reference to `NimBLEGluer::setPointer(int)'
                   //in that line there is NO reference to this class at all!
    //destructor
    //virtual ~NimBLEGluer();  

    int testPointer = 123;  //initial value for testing
    /*
    void setPointer( int inp);
    int getPointer( void ){
        return testPointer;
        };
    */        
    }; 

//NimBLEGluer::NimBLEGluer() {}  //causes a: error: definition of implicitly-declared 'constexpr NimBLEGluer::NimBLEGluer()'

//NimBLEGluer::~NimBLEGluer() {}

extern NimBLEGluer NimBLEData;  //to make it globally accessible without intantiating it again ?? 
/***************************************************
// in main() :

// in driver.cpp :

****************************************************/





#endif