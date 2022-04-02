# MIDI-sequencer

MIDI sequencer/recorder functions are a too heavy load for the Faust DSP application on a TTGO TAudio V1.6 board. The DSP task is already pretty heavy in it's own, so it may be useful to dedicate the firmware of the audio board as much as possible to it's core task: **synthesising audio**. 

A solution is to add additional functionality to a second board, that does not need to have an audio codec. This second board can send MIDI commands to the audio board over a suitable interface. The received timestamped MIDI commands are delivered in the right order and can be played "immediately", without further processing except for using a queue and timers for respecting the synchronized MIDI timing.

The MIDI sequencer board can be connected with the synthesizer board over bluetooth. Additionally, the sequencer board may have a wired (UART) connection with a MIDI instrument, such as a keyboard.




#### REMARKS:

Scan [ESP-ADF](https://github.com/espressif/esp-adf) for this kind of applications.   NOT AVAILABLE

ESP-IDF and MIDI:

- [MIDIBLE](https://github.com/0x0c/MIDIBLE)
- [blemidi-idf](https://github.com/mathiasbredholt/blemidi-idf)
- [blemidi](https://github.com/midibox/esp32-idf-blemidi)  might have the same functionality as the Arduino app below

[this](https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/optimizing-ble-midi-with-regards-to-timing-1293631358) is about applying BLE for MIDI

Arduino and MIDI:

- [ESP32-BLE-MIDI](https://www.arduino.cc/reference/en/libraries/esp32-ble-midi/) and the [repository](https://github.com/max22-/ESP32-BLE-MIDI)
- [BLE_MIDI.ino](https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino) (referenced by Mathias Bredholt (blemidi-idf)

Both examples (ESP-IDF and Arduino) are based on the [Arduino BLE-MIDI transport](https://github.com/lathoub/Arduino-BLE-MIDI) lib. Also have a look at this [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library). It may be wise to build the sequencer based on this in the Arduino environment. Allthough, then it is not possible to use thejdksmidi lib? Which can also be an advantage ;-)

For sequencing functionality, just use Google Arduino MIDI sequencer and you will find a lot, such as this [Old-School Arduino MIDI Sequencer](https://www.instructables.com/Old-School-Arduino-MIDI-Sequencer/) that may contain useful info on the wired connectivity.

### Step 1:  Create Arduino BLE app for testing

Running the 01-Basic-Midi-Device.ino  example on a TTGO Lora32 board.
See incoming MIDI data with the nRF Connect app on a mobile phone

### Step 2: Add BT/BLE client to the TTGO TAudio app

Add MIDIBLE to the Faust DSP firmware: faust_mqtt_tcp6_nb_v6
  - properly include the component (create components dir, add two CMakesList.txt files)
  - properly include the cpp_utils dependency (see [here](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils)
  - enable Bluetooth with idf.py menuconfig

- Working BLE (NIMBLE) on TTGO TAudio:  esp_idf\bleprph\main   
Example configuration ---->
    I/O Capability (Just works)  --->
[ ] Use Bonding
[ ] MITM security
[ ] Use Secure Connection feature

Note: the .ino app is probably not based on Nimble. Is it compatible with devices running Nimble BLE?  

More info on Nimble BLE:  [here](https://github.com/apache/mynewt-nimble)  
Basic server and client examples and a lot of information, [here](https://github.com/h2zero/esp-nimble-cpp)  

nRF Connect diagnostic tool on mobile can write and receive data from the app (on command of nRF Connect)

- 01-Basic-Midi-Device.ino on TTGO Lora32

nRF Connect diagnostic tool on mobile can write and receive data from the app (pushed by the .ino app)

How to connect both apps to each other????

### Create basic Nimble server and client examples

A lot of information about Nimble BLE, including examples can be found [here](https://github.com/h2zero/esp-nimble-cpp). The examples in the repo code (called esp-nimble-server and esp-nimble-client here) are different from those in the New User Guide (called esp-nimble-server2 and esp-nimble-client here2). Both examples run on the TTGO TAudio and the Heltec LORA32 boards. With the first ones data transfer has been shown.

Further investigations needed:  

1. which role can push data to the other one? We do not want to let the Audio board do polling!  Read [this 1](https://embedded.fm/blog/ble-roles) and specifically bullets 3 and 7 under data transfer.
2. how to show data transfer between server2 and client2?
3. look for the option that is lightest for the audio board.

Based on the [role definitions](https://web.archive.org/web/20160930015609/http://projects.mbientlab.com:80/bluetooth-low-energy-basics/) during creation of the connection and during operation a first guess for implementation of the MIDI synthesizer / sequencer application  would be let the **sequencer have the master and server** roles and the let the **audio board be the slave and client**. Note: master and slave are roles during making of the connection and client/ server describe roles after the connection has been established. Peripheral and Central are equivalents of slave resp. master.

Note: for compatibility,  make sure that these roles are compatible with existing applications, such as Android apps.
You can have a look at the configuration of existing MIDI BLE sound modules, e.g. [here](https://tigoe.github.io/SoundExamples/midi-ble.html) or [here](
https://blog.adafruit.com/2021/09/07/optimizing-ble-midi-with-regards-to-timing-bluetooth-midi-nordictweets/)  
https://blog.adafruit.com/2021/09/07/optimizing-ble-midi-with-regards-to-timing-bluetooth-midi-nordictweets/  
Have a look at something like a MIDI BLE specification.   
Because we have a semi-realtime application where we don't want the audio board give the task for polling for new events, we want the sequencer to push data to the audio board. So data transfer is done via the **notify** mechanism.
Now that role definitions are clear, the MIDI BLE specification is kind of clear, you have to know what should be the actual BLE role of a MIDI device of a type. try to collect pairs of MIDI ble apps to sniff the roles.

#### Sequencer app (Nimble master/Central and server)

- input: collect MIDI events from instruments
- output: send timestamped MIDI commands to a sound generating device

The output must be in the order that the events must be handled by a connected audio generating device.

This app will be built around the nimble_notify basic example from [this library](https://github.com/h2zero/esp-nimble-cpp) 
A working example is avaliable here as esp_nimble_notify_V2 

#### Audio/DSP app (Nimble slave/Peripheral and slave)

- Receive timestamped MIDI commands over BLE (Nimble)
- Execute MIDI commands in a synchronized way
- 
This app will be built around the nimble_client basic example from [this library](https://github.com/h2zero/esp-nimble-cpp) 
A working example is avaliable here as esp_nimble_client_V2 

## Implementation plan

1. Adapt the esp_nimble_notify_V2 example to send a a stream of single notes.
2. Add the client part to the TTGO TAudio board demonstrate receipt of the data
3. DONE (faust_ble_midi_v1) Add the BT client to the Faust DSP application: faust_ble_midi_v1   had to dismantle WIFI and MQTT to reduce firmware size
4. DONE (faust_ble_midi_v1) Show incoming Nimble MIDI commands from the sequencer, while playing the metronome on the soundboard  
5. DONE (faust_ble_midi_v2) Demonstrate playing of the single notes based on incoming commands
6. DONE (faust_ble_midi_v2) Add ESP logging to the sequencer
7. DONE (faust_ble_midi_v2) Add WIFI/MQTT functionality to the "sequencer" to communicate with Nodered in the same way as the Faust DSP app now does (version faust_mqtt_tcp6_nb_v5)
8. DONE (fckx_seq_v3) Demonstrate playing single notes entered via Nodered (use 5 byte buffer in Nodered), play immediately in API (/fckx_seq/midi/single)
9. Re-use the browser-based Faust GUI for the sequencer. Just use appropriate MQTT topics  
10. Add more sequencing functionality, using e.g. the jdksmidi library. Be compliant with the MIDI specifications
11. On success (matching the functionality of the current faust_mqtt_tcp6_nb_v5 firmware) remove the basic sequencing functionality from the DSP firmware
12. Introduce visible or audible feedback to the user about BLE advertising and connection status
13. Play MIDI from the web or a file
14. Introduce interface with MIDI instrument, sucha the Yamaha PSR 500
15. 



## Input / output specifications

#### Starting points

1. The primary application for the sequencer is to act as source of input for synthesiser applications on the TTGO TAudio board
2. Besides it's main task of generating digital synthesized audio, the TTGO TAudio board offers limited capabilities for processing of incoming commands
3. Communication between the audio board and the sequencer should as much as possible comply with existing standards

Based on the above, we start with defining the spec for the sound board and derive sequencer specs from that.

### Sound board I/O specifications

- accepts MIDI commands in order of time that they should be played  
- accepts timestamped (delta-t) MIDI, at first (and preferably), the time stamps will be neglected. MIDI ommands are executed immediately, without the need for further buffering, timing or processing  
- accepts MIDI commands over Bluetooth. Because it is a light-weight protocol, BLE will be used. The soundboard will act as the xxxx (connect to the sequencer when it is advertising a connection)
- for the same reason, NimBLE will be used for implementation of the Bluetooth connection
- the received MIDI data must be encoded following the MIDI-BLE standard (5 bytes, with in the first two bytes a header, containing 13 bits for a timestamp  
- at the start, the sound board only accepts keyOn (status 0x90) / and keyOff (status 0x80) commands, that will be played immediately. Timestamps are neglected
- there is also a need to control settings such as ADSR and other DSP parameters for the synthesizer
- there is also a need to control settings for the audio codec   

### Sequencer I/O specifications

**Output**
- Offers a Bluetooth service (NimBLE implementation)
- output MIDI data are encoded following the MIDI-BLE standard
- MIDI data are sent in order of playing time
- supply control settings for DSP parameters for the synthesizer this can (preferably) be done as MIDI commands
- supply control settings for the audio codec to sound board
- (As of V33) send controller settings to GUI   


**Input**
- can receive "real time" input over MQTT 
- (As of V33) can receive input from soundboard about actual values of DSP and codec controls. Required for syncing GUI elements with these settings
- later a connection to a musical instrument is envisioned

## Sequencer mode(s) of operation

- can accept real time input 
- can forward the real time input immediately to its output (can be switched off or on)
- can store incoming data
- can simultaneously send stored data to it's output, e.g. in a loop

- data are stored in a cue with timestamped MIDI messages  
- for efficient processing or output, the way of storage (data encoding and storage structure) must enable sorting in order of time  
- timestamps in the cue must be suitable to span long times (minutes/hours). A 4 bytes timestamp that represents milliseconds or processor ticks (10 ms each) seems appropriate  
- for storage and handling have a look on various applications on the web (jdksmidi MIDI-Nimble ??) 

## Sequencer implementation

As the operation involves a number of different tasks that are also time-critical, the sequencer implementation is based on using freeRTOS elements. This includes tasks, timers but may also include freeRTOS queues for efficient handling and communication between the tasks (under investigation). Important aspects: can a freeRTOS queue be sorted? Can you insert an element into a freeRTOS queue in a position of your choice? Can you hack cues to mimick that?

- Have a look at jdksmidi for MIDI sequencer related classes. The project structure is complex. The code is out-dated and does not have a living community.  
- A modern alternative ~~based on~~ inpired by it is [NicMidi](https://github.com/ncassetta/NiCMidi). Also see the [docs](https://ncassetta.github.io/NiCMidi/docs/html/) for it. The code is not directly transferrable to ESP-IDF (uses <thread> and in timer.h and uses RtMidi for message I/O)
- In the end, successfully added examples of NiCMidi's MIDIMessage and MIDITrack (v6.b -> 7). All elements relating to MIDITimer switched off.

### Sequencer tasks  

Task in *italics* still to be implemented
  
1. *Maintain a MIDI clock / beat*  
2. *Output commands for an audible metronome*  
3. Temporary storage of incoming events in order of receipt. This involves adding a timestamp representing the **moment of receipt**
4. Send MIDI commands to the output for immediate playing. This may involve an output buffer that is emptied as fast as possible over the NimBLE interface. Note: this can involve commands that have just been received (MIDI through) or commands that are output by e.g. a looping task.

4b. *Cast incoming MIDI into the MIDIMsg type*  
    
*OPTION 1:*  
5. *Append (incoming) commands to an input queue with a timestamp for the moment of receipt*  
6. *Insert each message from the input queue into a **track** corresponding with it's channel number. After completing this task all tracks are in order of intended moment of execution (i.e. in order of the timestamps in this queue). When e.g. recording in a loop, this involves conversion of the timestamp of receipt in a timestamp for execution. This conversion depends on implementation of Task 1*    

   
*OPTION 2:*    
5. *Insert (incoming) commands directly into a **track** with a timestamp,  at the position representing it's timestamp (possibly adapted e.g. to fit it in a playing loop)*  


The option (2) for having a queue that is always sorted is attractive, but may be time critical.  
It may become less time critical, when an input buffer is used for later insertion (option 1).  

#### Progress on tasks

- **Task 3** introduced storage of incoming MIDI messages in inQ. Timestamps (in system ticks) are added immediately at receipt. Messages are in order of receipt so in order of system time (fckx_sequencer_v4). This task is fired at incoming MIDI events over MQTT.  
- **Task 4** done, beit in a dirty way. To be improved  
- **Task 4b** TODO NOW  
  
- **Task 1** considerations:
  - basic unit is the system time tick (currently 10 ms, but may need to be reduced in case of audible hick ups, see MIDI and BLE-MIDI specifications
  - MIDITime is nr of system ticks since reboot or since connection to Nimble client (a kind of session time) (t.b.d.)
  - As system time, MIDITime is continuously increasing
  - The "beat" is governed by the tempo (e.g. 120 bpm) and the time signature (e.g. 4/4, 3/4, 5/4 or more exotic signatures)
  - The "beat" is defined as the number of quarter notes per minute
  - A time signature is represented as time_sig_denom/time_sig_num (e.g. 3/4). Where time_sig_denom defines the number of notes in a measure and 1/time_sig_num the length of a note.
  - Loops for recording / playing always start at the start of a measure and contain a number of full measures
  - Multiple simultaneous loops can be playing, each driven by it's own task. 
  
 ### Implementation / testing of NiCMidi library
  **OOPS deleted the wrong folder, without intermediate commits....  I have to redo updates in v7 to achieve the results below !**
  
 fckx_sequencer_v7 contains 3 examples. The first 2 are from the docs. The third is from an example file in the repo
  
  1. MIDIMessage
  2. MIDITrack, DumpMIDITrack
  3. MIDIManager, MIDITimer, MIDITick  has runtime error on stopping MIDITick component
  
  **REMARKS**
  - Have a look at a comment in test_component.cpp:
    
   *If you want to implement your own MIDITickComponent derived class you must at least redefine the StaticTickProc() and TickProc() methods (and probably Start() and Stop() also). Before using the class you must add it to the MIDIManager queue with the MIDIManager::AddMIDITick().*
  
  In manager.cpp removed references to RtMidiIn and RtMidi out. The file is renamed to maager_dirty.cpp. The changes in _dirty files are mostly (if not always) indicated by a //FCKX tag
  
  IN the example, removed these calls:
  MIDIManager::GetOutDriver(0)->OpenPort();
  MIDIManager::GetOutDriver(0)->ClosePort();
  
  The files had to be adapted, partly because I had problems with creating a proper project structure / includes.
  More important: references to MIDIOut and MIDIIn and RtMidi have been commented out of the code.  Adapted file names have an appended '_dirty' 
  
  **Status in version 7:** 
  
Partly executing the example, untill a runtime error is thrown at removal of the tick component  
(**resolved in v8** by only PARTLY BYPASSING Init of the tick component (only calls to RtMidi)  i.s.o. COMPLETELY)
  
  
E (2939) APP_MAIN: Testing NiCMidi functionality: MidiMessage                                    //erroneous message, repaired in v8
Starting the component ...
Waiting 10 secs ...
Stopping the component ...
Waiting 5 secs without playing ...
Exiting
Executing MIDIManager::Init() BYPASSED !!! contains calls to RtMidi                              //is Init called at Exit???
                                                                                                 //YES, at bool MIDIManager::RemoveMIDITick(MIDITickComponent* tick)
Guru Meditation Error: Core  0 panic'ed (LoadProhibited). Exception was unhandled.

Core  0 register dump:
PC      : 0x400d5d66  PS      : 0x00060e30  A0      : 0x800d70d3  A1      : 0x3ffbb450
0x400d5d66: MIDIManager::RemoveMIDITick(MIDITickComponent*) at c:\users\fred\.espressif\tools\xtensa-esp32-elf\esp-2020r3-8.4.0\xtensa-esp32-elf\xtensa-esp32-elf\include\c++\8.4.0\bits/stl_vector.h:806 (discriminator 1)
 (inlined by) MIDIManager::RemoveMIDITick(MIDITickComponent*) at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/../main/manager_dirty.cpp:229 (discriminator 1)

A2      : 0x3ffbb4b0  A3      : 0x00000000  A4      : 0x3ffc3b98  A5      : 0x00000000
A6      : 0x00000000  A7      : 0xff000000  A8      : 0x800d5d5d  A9      : 0x00000000
A10     : 0x00000000  A11     : 0x0000000a  A12     : 0x00000007  A13     : 0x00000000
A14     : 0x00000000  A15     : 0x00000001  SAR     : 0x00000016  EXCCAUSE: 0x0000001c
EXCVADDR: 0x00000000  LBEG    : 0x400014fd  LEND    : 0x4000150d  LCOUNT  : 0xffffffee

Backtrace:0x400d5d63:0x3ffbb450 0x400d70d0:0x3ffbb470 0x400d4e30:0x3ffbb490 0x400d5680:0x3ffbb4b0 0x400d5b1b:0x3ffbb520 0x400d39ab:0x3ffbb640
0x400d5d63: MIDIManager::RemoveMIDITick(MIDITickComponent*) at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/**../main/manager_dirty.cpp:229** (discriminator 1)

0x400d70d0: MIDITickComponent::~MIDITickComponent() at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/**../main/tick.cpp:32**

0x400d4e30: TestComp::~TestComp() at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/../main/main.cpp:825

0x400d5680: main at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/../main/main.cpp:912

0x400d5b1b: app_main at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v7\build/../main/main.cpp:1033

0x400d39ab: main_task at C:/Users/Fred/esp-idf/components/esp32/cpu_start.c:600
  
  **Status in version 8: no runtime error** 
  (removed INFO logs of the MQTT client and handlers)
  
E (2819) APP_MAIN: Testing NiCMidi functionality: MIDItimer MIDITickComponent, MIDIManager
E (2899) APP_MAIN: Testing NiCMidi functionality: test_component.cpp
Starting the component ...
Waiting 10 secs ...
Stopping the component ...
Waiting 5 secs without playing ...
Exiting
Executing MIDIManager::Init() PARTLY BYPASSED !!! contains calls to RtMidi
Executing MIDIManager::Init()
Exiting MIDIManager::Init() Found 0 midi out and 0 midi in

**Try to "silently" play the notes. Only show the screen logs. These are missing now. It looks like the TickProc is not called.
  This is most likely due to the fact that MIDIManager::AddMIDITick(&comp); is commented.

**Status in version 9: no runtime error**  
Plays audible note to soundboard, without runtime errors!
Need to check the MIDI codes that arrive at the board. Added logging of received MIDI messages to it's firmware.  
  
  **Next steps:**
  
  - DONE v9 Debug the MIDITick component  
  - DONE v9 Wrap MQTT port in MIDIIn,  wrap BLE port in MIDIOut,  phase out RtMidi (?)
  - wrap the bluetooth (NimBLE) MIDI output port in driver.h/.cpp
  - implement more MIDITick users (sequencer, recorder)
    - implement commands for e.g. MIDIManager (start, stop, etc. .... ) over MQTT
  - clean up the code
  
  - soundboard:  extend controls
    MIDI:
    - AllNotesOff
    - Channel selection
    - 
    DSP:
    - other sound engine controls (e.g. A, D, S, R, ...)
    
  - soundboard:  control audio codec over BLE interface
  - implement a GUI facilitated by the GUI classes in the lib. Handle these on the Nodered side as with Faust JSONUI
  
### Wrap the bluetooth (NimBLE) MIDI output port in driver.h/.cpp
  
In the TickProc for the test_component example the out of MIDI messages over the NinBLE bluetooth interface is done by sendToMIDIOut(msg) instead of  MIDIManager::GetOutDriver(0)->OutputMessage(msg). If we modify the code behind GetOutDriver to use the NimBLE interface we prevent that we have to adapt the call in the entire library.
  
manager.cpp / class MIDIManager uses a call static MIDIOutDriver*       GetOutDriver(unsigned int n)  to get a pointer to the output driver
  

## Change of patching strategy
  
  The patched NiCMidi has become a pretty mess in v10. Files have been moved, renamed etc.
  **New strategy**: 
  - place the intact NicMidi lib in the structure as a component
  - only add a patch inside files
  - do not move or rename files to prevent a cascade of changes
  - just place a //FCKX patch near the change, so you can find them back with Search Everthing 
  - **REDONE** the work starting form v6b or 7 until you reach the status of v9
  - implement test_recorder.cpp, test_midi_ports, test_metronome in v10 NEW (take over useful snippets from v10)  recorder only the global and recorder instantiation, not the calls ( main() ) 
  - compiles OK, but firmware size too large
  - v11 reduce logging level INFO > WARNING no success
  - v11 reduce logging level INFO > ERROR SUCCESS! BUT, run time error thrown by sequnecer.cpp:
  
  
Executing MIDIManager::Init()
MidiOutDummy: This class provides no functionality.
MidiInDummy: This class provides no functionality.
Exiting MIDIManager::Init() Found 0 midi out and 0 midi in

abort() was called at PC 0x4014ba8f on core 0
    
0x400dbd2f: MIDISequencer::MIDISequencer(MIDIMultiTrack*, MIDISequencerGUINotifier*) at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v11_new_no_rtmidi\build/../components/NiCMidi/src/sequencer.cpp:361

0x400d5d35: AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier*) at c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v11_new_no_rtmidi\build/../components/NiCMidi/src/advancedsequencer.cpp:114
  
sequencer.cpp:361  :  
    if (!MIDIManager::IsValidOutPortNumber(0))  
        throw RtMidiError("MIDISequencer needs almost a MIDI out port in the system\n", RtMidiError::INVALID_DEVICE); 
  
It is time to bypass RTMidiError and use the NimBLE interface... 
  
  
  
The dirty option:  call Nimble if such a throw is on hand
Better,  replace calls to RtMidi by call to your own RtBLEMidi class !  
  
BE AWARE of the limited firmware space!
  There is probably room for 4 MB!  Have a look at partition settings! DONE!
  
Have a look at void MIDIManager::Init()  and provide your own RtBLEMidi

### Next step in implementation  
  
V12 contains all (yet empty) API functions for NimBLE via RtMidi (dirty/hacked version). NimBLE out has been phased out of the main app and should be taken over by NiCMidi/RtMidi.  Implement the API functions an make sure you maintain the same interface.  
  
## ~~I Give In~~:  no success...YET
  Most versions before V13 are with RtMidi.  V13 is without, buth this is too complicated..... see remarks by NicMidi (211203)  
  V13 route: rewrite NiCMidi driver to **access already** running nimBLE driver  
  
  V12 route:  use RTMidi, but give it access to **already running** nimBLE driver via a GLOBAL class  
  Strange error in V12:  /main/main.cpp:563: undefined reference to `NimBLEGluer::NimBLEGluer()'     There is no code ther AT ALL referencing NimBLEGluer ! ????  
    
  Both routes probably need a global object that contains driver info after this has been instantiated / started     
  - https://docs.particle.io/cards/firmware/global-object-constructors/global-object-constructors/ !!! 
  - https://stackoverflow.com/questions/297822/global-instance-of-a-class-in-c  
  
  v12 now contains a first VERY BASIC version of nimBLEOutdriver. The object is globally accessible and SHOULD NOT be instatiated again. It was intanstiated in the header file using extern  xxxxxxx nimBLEOutdriver
  
  The actual definition of the nimBLE server / peripheral (advertising device)  and creation of a connection must still be implemented. Ongoing in v14....
  Until autonomous creation of a connection is demonstrated, call to NicMidi examples (recorder/sequences) that use the connection will not be started. Other NicMidi examples are executed before the program jumps into its eternal waiting loop.    
  
  The essential reason to let the sequencer have the role of BLE peripheral is that only with this role it can push messages to other devices, without the need for confirmation/ACK
  
  v14: correct instantiation of nimBLEdriver via the .... class .  Server starts advertising and accepts connection with peer  
  v15:  
    - to be done: cast this functionality properly in the NicMidi interface  
                  testing this implementation is done with the NiCMidi component_example (tick component) as it is relatively simple. There is focus on output in that example.  
    - done:   
      - proper initialisation of output port  
      - connectedTask (wrong name! but this is a heritage of the nimBLEDevice  "notify" example) was  moved to MidiOutNimBLE class in nimBLEdriver.cpp.  
      - the loop in connectedTask does not yet contain actions for the case that there is a connection  
     - it properly reports connection / disconnection events  
   - to be done:   
     - add meaning full content to each of the parts of the connectedTask. You probably have to add callbacks. See how NicMidi wants to use the connection  
     - recognize connection to nRF Connect tool and do not assign MIDI functionality to that  
     - implement sendMessage 
  
  ###Success! 
  
   **v16:**  
  - sends 3 byte messages generated by the test_component example to the peer device 
  - v3 of the faust_ble_midi synthesiser firmware adapted to accept 3 byte messages (in stead of the 5 byte BLE-MIDI messages)
  - v16 code is now quite messy. Cleanup postponed until after exploring more NiCMidi functionality.
  - See TODO for a non-prioritized list of actions
  
  **v17:**  
  - added copies of nimBLEdriver.cpp/.h to serve as templates for MQTTdriver.cpp/.h .  This now contains the barebone MQTTMidiIn class, with bindings to NiCMidi manager and driver. Compiles OK. Running without errors. Actual MQTT in to be implemented.  
  
  **v18:** 
  - fresh version for implementation of MQTT Midi in.
  - define an MQTT topic and handler for Midi messages
  - let openPort/closePort do the subscription/unsubsription
  - in some way bind the MQTT handler to the (protected) HardwareMsgIn of the MIDIInDriver class. As it is protected: don't touch it. Access it via the instance of the class. Share common data (the message) with the MQTT handler. 
    - note 1: the msg is entered into the HardwareMsgIn handler via the MIDIInDriver class (in what direction is the flow of input information ?????)  
  NO! it is a callback! the callback must be activated (preferably) directly by the MQTT event (see note 4 d) 
    - note 2: Use port->setCallback(HardwareMsgIn, this) to set the callback  
    - note 3: Investigate what the port->ignoreTypes(false, true, true); does. Does it switch of certain Midi message types?  
    - note 4: Nice milestones ----->  
      - a implement openPort  in MQTTdriver
      - b implement closePort in MQTTdriver 
      - c check proper implementation by using isPortOpen 
      - d implement port->setCallback(HardwareMsgIn, this); in MQTTdriver  look in RtMidi init for how to set the callback
      - e implement port->ignoreTypes(false, true, true); in MQTTdriver
      - f check triggering the HardwareMsgIn callback by detecting the output of: std::cout << drv->GetPortName() << " callback executed   "; 
  NOTE!!!! you probably have to use   
  MQTTMidiInData *data = static_cast< MQTTMidiInData *> (inputData_);  
  or an equivalent to locally (within the scope of a function) have a reference to the mididata and buffer
  Why isn't this done with a lasting reference?
  Possibly because is connects the hardware buffer with the "software" part?????
  V19:  fresh version for further implementation of MQTT input.  Forget the version 19 SKIP version. It is messy.
  Important to do first:  remove depedencies on RtMidi.  This is probably only in driver.h
  This will give you a clearer view on shortcomings of your own drivers.
  
  **v19:** 
   - made HardwareMsgIn temporary public to allow direct calls i.s.o. using it as a callback
   - introduced direct call to HardwareMsgIn in MQTT event handler. System reports receipt of messages over MQTT:
  
    E (67276) FCKX_SEQ_API: SEND MQTT INPUT VIA MIDIManager::GetInDriver(0)->HardwareMsgIn TEST DIRECT CALL (no callback)
    W (67286) NICMIDI DRIVER: A sign of life from HardwareMsgIn (make it protected again in driver.h!!!)
    MQTT_In callback executed   Got message, queue size: 3
    E (67306) FCKX_SEQ_API: Learning about MIDIManager Interface
    E (67306) FCKX_SEQ_API: MIDIManager::GetNumMIDIIns() 1
    E (67316) FCKX_SEQ_API: MIDIManager::GetInDriver(0)->GetQueueSize() 3
    E (67326) FCKX_SEQ_API: MIDIManager::GetInDriver(0)->CanGet() 1
  
  **v20:**
   - tested response of recorder on current status of MQTT input
   - sequencer example plays a note on receipt via MQTT, but keeps on playing at every miditick (input queue NOT flushed).
   - V20a, repaired issue with wrong data. Removed usage of dummy note for testing.
  
  **v21 TASKS AHEAD:**
   - SOLVED issue with repeated usage of data in input queue. Input queue is not emptied after using the data because FlushQueue was commented in manager.cpp:255
  
  **v22:**
   - DONE Inspect data in tracks (changed the used track dumping routine). See notes in main.cpp.
  
  **v23:**  
   - Try to record notes in a loop and play them
   - Switch off / control metronome in recorder example  NOTE: metronome plays on channel 9, maybe can mute a channel?
   - Allow usage of AllNotesOff command to ease the developer :-)
   - IF OK: convert HardwareMsgIn to a real callback (triggered by ...receipt of a message.... ? )
   - Critically evaluate input of MIDI data in sequencer.  Nordered MQTT sends 5 byte timed BLE_MIDI format
     sequencer MIDI in (MQTT API / HardwareMsgIn uses only 3 bytes now but adds a timestamp on receipt.
   - Decide on usage of timestamp on send or timestamp on receipt. Keep both methods as options. Note:  if using timestamp on send, both clocks need to be synchronized.
   - Investigate output of data via MQTT, using the ost option in dump_tracks.h/.cpp .  Also see http://videocortex.io/2017/custom-stream-buffers/ for implementation of ostream. 
   https://codereview.stackexchange.com/questions/185490/custom-ostream-for-a-println-like-function
  
  **v24:**
   - reverted to playing a song (twinkle twinkle) instead of recording
   - switched to different board (TTGO TAudio) and some other settings in menuconfig (SPI / external memory) NEED TO COMPARE SDCONGFIG OF V24 and V23 to see those!
   - implemented some commits by NiCMidi to repair bugs: 
        Bug on autostop corrected
        Found a bug when the MIDISequencer auto stops, and corrected it.
        This affects files sequencer.h, sequencer.cpp and advancedsequencer.cpp.
        Minor changes in notifier.h and processor.h
        commit 92d5c2ab5ead6399e2b79da905139c4a6daebd6a  
  
   - NEW TODO's:

  
**v25:**
  - DONE create more stable connection procedure to nimBLE output (initialize before any call to NiCMidi)
  - repaired bugs in NiCMidi

**v26:**  
  
**v27:**  
  - improvements in creation of nimBLE connection (sequencer example is the test case, still not flawless
  
**v28:**  
  - implemented modifications in Nic's commit of dec 29th.
  - TEST_SEQUENCER part now runs pretty smoothly. Some hick-ups when playing in a loop. To be analyzed.
  
**v29:**
  - test recording
  - analyze hick-ups in sequencing loop 
  - SOLVED ISSUE with crashes in sequencer example.  Linked PThreads to CORE 1 in ESP-IDF menuconfig

**v30:**
  - test recording  
  - SOLVED non-fatal errors in thru and non-exiting from recording mode by INCREASING Midi timer tick resolution from 10 to 50 ms (tick.cpp:50)
  
**v31:** 
  - The "solutions" in versions 29 and 30 were no real solutions. They only reduced the occurence of the instability.
  - In v31 the issue was solved by introduction of a timer based on a freeRTOS timer rather than on a thread. See timer.cpp/.h
  - The solution is specific for ESP32 (ESP-IDF) systems and can be selected by a conditional complilation flag ESP32_TIMER
  - The timer resolution must be a multiple of the freeRTOS tick resolution (available as portTICK_PERIOD_MS) in the code
  - portTICK_PERIOD_MS can be changed in the freeRTOS component section in ESP-IDF menuconfig.
  
  **v32**
  
  - solved issue with lost messages in thru mode
  - implemented midi commands for preset selection (0xCn 0xpp)
  - implemented midi commands for controller settings (0xB0 0x67 - 0x71)
  
  **v33**
  [syncing GUI between sequencer and synth](https://github.com/goofy2k/MIDI-sequencer/blob/main/docs/syncing%20of%20gui%20controls.md)  
  - test messages arrive over onWrite CharacteristicCallBack in main. 
  
  **v34**  
  [syncing GUI between sequencer and synth](https://github.com/goofy2k/MIDI-sequencer/blob/main/docs/syncing%20of%20gui%20controls.md)
  - implemented exchange of controller settings
  
   **v35**  
   Inititial foundation for handle_seq_command dispatcher (seq GUI)
  
   **v36**
   First version with recorder / advanced sequencer example under control of Nodered GUI (ifdef TEST_RECORDER)
  
   **v37**
   Replaced hard-coded thru in recorder by UI/GUI controllable thru in TEST_RECORDER example
  
   **v38**  
  Created separate NimBLE characterisctic (pGUICharacteristic) for exchange of GUI data with synth.  
  Test messages over this channel can be sent from the main of the synth app. 
   
  
   **v39**  
  Implemented a customized ostream (via it's streambuf of class class Outbuf_buffered_fckx) for NicMidi notifications over MQTT. NicMidi notifier.h/.cpp was modified. Now it contains a class MIDISequencerGUINotifierRaw that outputs the raw GUI event data (unsigned long int), This can be used as a replacement for class MIDISequencerGUINotifierText that outputs notifications as text derived from the raw data.
  
  The notification raw data are sent to the Nodered FCKX_SEQ GUI flow. There, the raw data is converted to readable text that is displayed in a scrolling window in the Midi Sequencer dashboard. A next step is to use the GUI raw data notification to control a GUI (HTML/Javascript under Nodered). 
  
  **v40**
  
  
  
  
  TODO
  - Feed back / synchoronize status of some controls (MIDI Thru / Start recording / ... ) between sequencer and Nodered dashboard
  - A next step is to use the GUI raw data notification to control a GUI (Javascript under Nodered)
  - ONGOING Check editing of presets / usage of keyBoard 2
  - DONE from v39: Make MQTT backtalk from seq to Nodered for sending GUI notifications. 
  - DONE from v38: Enable feedback from synth to seq with NimBLE characteristic onWrite. Currently it is only in TEST_THRU
  - DONE from v38: create separate GUI element to control thru. It is no started by default when recorder is started
  - DONE from v37:  replace hard-coded thru in recorder by UI/GUI controllable thru in TEST_RECORDER example 
  - DONE V34 for DSP presetUnderEdit  : synchronize DSP controller settings between GUI server and synth board (requires additional NimBLE communication with synth board)
  - synchronize WM8978 controller settings between GUI server and synth board 
  - NimBLE write from synth board to sequencer board has been implemented in THRU example. Investigate if generalisation is required. 
  - check recorded notes (pitch, channel). This becomes easier after switching of metronome in recorder OR switching of metronome logs in synth
  - analyze / improve reliability of MIDI In (MQTT).  Lost messages and timing accuracy.
  - move the setting of timer tick resolution to outside tick.cpp, preferably to app_main
  - DONE v31 try to remove the option #define DEFAULTSTATICSTOPPROC  sequencer.cpp:31
  - find ways to increase the resolution again, e.g. by analyzing/ balancing CPU load
  - it looks like the NiCMidi notifier output is delayed irregularly, this may hamper future GUI smoothness 
  
  - implement other channels than 0 in synth app
  - make channels > 1 recognizable in synth (e.g. by changing some FaustDSP settings) NEW REPO CREATED FOR SYNTH APP:  Faust-Synth-MIDIBLE
  - try to uncomment playing other channel numbers (uncomment line ~1514 and further, "THE REST IS COMMENTED OUT")
  - try to revert back to Heltec LORA32 hardware
  
  1. clean up fckx_sequencer code **STARTED v17**: needs continuous attention.  
  2. NimBLE output driver causes duplicate PRIMARY BLE service  **SOLVED** by moving all code from initialisation to openPort  
     This means that the first instantiation is only used for determining the number of ports (which is always 1). The real initialisation is postponed to the opnPort routine which is explicitely called by MIDIManager. If openPort has become too heavy by this, see if some of the code can be moved back to initialisation. 
  3. nRF Connect sniffer reports "Indication received from ... " whereas the intention is to send a notification.  **SOLVED** removed the indication flag form the port setup
     Note that indication is a protocol that requires acknowledgement by the receiver. It is also slower than notification, which is effectively pushing.  
  4. test more NiCMidi functionality (recorder, sequencer, midi-thru).  **STARTED v17** with implementation of MIDI in via MQTT  (MQTTdriver.cpp/.h)  
  5. study suitability of BLE-MIDI standard (5 byte timestamped) vs NiCMidi 3 byte messages (does NiCMidi offer 5 byte messages)
  6. implement more message types (other than "note on", "note off"  on the synth side. AllNotesOff would be fine :-)
  7. implement recognition of MIDI channel in the synth firmware. This enables usage of multiple synths.
  8. Does NicMidi need the abilty to define callbacks for the "while connected" case , or is availability of send message sufficient?
  9. Harmonisation / adaptation of NiCMidi to accept NimBLE output driver
  10. 
    a. Implement MQTT input driver for sequencer (useful for testing of recorder functionality)  (see 4.) REMARKS also inside driver.cpp 
    b. Implement nimBLE Midi IN for seqencer app.  Testing is possible with a second board running e.g. the test_component example  
  11. More items in the list a few paragraphs back
  12. See what to do with the blocking of the MIDIMAtrix out_matrix instantiation in driver.h and driver.cpp ( #ifdef DRIVER_USES_MIDIMATRIX). Blocking this may be the cause of the missing key off in the tick component example !

  

Re 10a and 4. : 
- uint8_t midiPacket already defined in nimBLEdriver.cpp  
 PROBABLY WISE TO CREATE/RE-USE A STRUCTURE/CLASS WHERE MQTTdriver and niBLEdriver inherit this (AND MORE) from  
- same for connectedTask (already defined). But in this case: MQTT probably doesn't need this a much is event-driven. This is also logical fr the input side.
  So: commented out in MQTTdriver.cpp  
- deviceConnected  double definition, commented out in MQTTdriver.cpp.  Reconnect for MQTT is probably already covered in MQTT part  

  
  ## Potential UI / GUI items (not complete)
  
  - Mode select
      - Thru
      - Sequencer
      - Recording
  - Thru controls
      - input channel (n or -1)
      - output channel (n or -1)
      - select processor(s)
  - Sequencer
      - tracks select
      - channels select
      - tempo
      - time signature
      - metronome on/off
      - metronome notes / bank
  - General
    - IO select / security
  
  
## Sequencer commands
  
  Are issued over MQTT with topic /fckx_seq/command
  The message payload consists of byte sequences, pretty much lik MIDI commands
  The following table shows the commands
  
| Nr| Code  | Description        |   main code line                   |   |   |
|---|-------|--------------------|------------------------------------|---|---|
| 1.|  0x01 |  start recording   |   recorder.Start()                 |   |   |
| 2.|  0x02 |  stop recording    |   recorder.Stop()                  |   |   |
|   |  0x03 |                    |                                    |   |   |
|   |  0x04 |  dump recorder     |                                    |   |   |
| 3.|  0x11 |  play              |   sequencer.Start()                |   |   |
| 4.|  0x12 |  stop              |   sequencer.Stop()                 |   |   |
| 5.|  0x13 |  rewind            |   sequencer.GoToZero()             |   |   |
| 6.|  0x14 |  dump sequencer    |                                    |   |   |
| 7.|       |                    |                                    |   |   |
| 8.|       |                    |                                    |   |   |
| 9.|  0x21 |  start thru        |                                    |   |   |
|10.|  0x22 |  stop thru         |                                    |   |   |
|11.|       |                    |                                    |   |   |
|12.|       |                    |                                    |   |   |   
|13.|       |                    |                                    |   |   |
