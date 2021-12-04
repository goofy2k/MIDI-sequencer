/* implementation of class MidiOutNimBLE */

/*
class MidiOutNimBLE {
    public:
                                    MidiOutNimBLE();
                                    ~MidiOutNimBLE();
                                
        void                         openPort(unsigned int portNumber=0);
        void                         closePort();
        virtual bool                 isPortOpen();
        unsigned int                 getPortCount() { return 1; }
        std::string                  getPortName(unsigned int portNumber=0);
        void                         sendMessage(const std::vector<unsigned char> *message);
}
*/
/*
FCKX: make sure that the members of this class correpond to the state and state changes
that are expected by the equivalent class(es) in RtMidi and NicMidi
*/

#include "nimBLEdriver.h"

/* Use NimBLEDevice to implement class MidiOutNimBLE */
#include <NimBLEDevice.h>
