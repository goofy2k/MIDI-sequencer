# MIDI-sequencer

MIDI sequencer/recorder functions are a too heavy load for the Faust DSP application on a TTGO TAudio V1.6 board. The DSP task is already pretty heavy in it's own, so it may be useful to dedicate the firmware of the audio board as much as possible to it's core task: **synthesising audio**. 

A solution is to add additional functionality to a second board, that does not need to have an audio codec. This second board can send MIDI commands to the audio board over a suitable interface.The received timestamped MIDI commands are delivered in the right order and can be played "immediately", without further processing except for using a queue and timers for respecting the synchronized MIDI timing.

The MIDI sequencer board can be connected with the synthesizer board over bluetooth. Additionally, the sequencer board may have a wired (UART) connection with a MIDI instrument, such as a keyboard.




#### REMARKS:

Scan [ESP-ADF](https://github.com/espressif/esp-adf) for this kind of applications.   NOT AVAILABLE

ESP-IDF and MIDI:

- [MIDIBLE](https://github.com/0x0c/MIDIBLE)
- [blemidi-idf](https://github.com/mathiasbredholt/blemidi-idf)
- [blemidi](https://github.com/midibox/esp32-idf-blemidi)  might have the same functionality as the Arduino app below

Arduino and MIDI:

- [ESP32-BLE-MIDI](https://www.arduino.cc/reference/en/libraries/esp32-ble-midi/) and the [repository](https://github.com/max22-/ESP32-BLE-MIDI)
- [BLE_MIDI.ino](https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino) (referenced by Mathias Bredholt (blemidi-idf)

Both examples (ESP-IDF and Arduino) are based on the [Arduino BLE-MIDI transport](https://github.com/lathoub/Arduino-BLE-MIDI) lib. Also ave a look at this [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library). It may be wise to build the sequencer based on this in the Arduino environment. Allthough, then it is not possible to use thejdksmidi lib? Which can also be an advantage ;-)

For sequencing functionality, jus use Google Arduino MIDI sequencer an dyou will find a lot, such as this [Old-School Arduino MIDI Sequencer](https://www.instructables.com/Old-School-Arduino-MIDI-Sequencer/) that may contain useful info on the wired connectivity.

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
4. DONE (faust_ble_midi_v1) Show incoming Nimble MIDI commands from the sequencer , while playing the metronome on the soundboard  
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

### Starting points

1. The primary application for the sequencer is to act as as source of input for synthesiser applications on the TTGO TAudio board
2. Besides it's main task of generating digital synthesized audio, the TTGO TAudio board offers limited capabilities for processing of incoming commands
3. Communication between the audio board and the sequencer should as much as possible comply with existing standards

Based on the above, we start with defining the spec for the sound board and derive sequencer specs from that.

#### Sound board I/O specifications

- accepts MIDI commands in order of time that they should be played  
- accepts timestamped (delta-t) MIDI, at first (and preferably), the time stamps will be neglected. MIDI ommands are executed immediately, without the need for further buffering, timing or processing  
- accepts MIDI commands over Bluetooth. Because it is a light-weight protocol, BLE will be used. The soundboard will act as the xxxx (connect to the sequencer when it is advertising a connection)
- for the same reason, NimBLE will be used for implementation of the Bluetooth connection
- the received MIDI data must be encoded following the MIDI-BLE standard (5 bytes, with in the first two bytes a header, containing 13 bits for a timestamp  
- at the start, the sound board only accepts keyOn (status 0x90) / and keyOff (status 0x80) commands, that will be played immediately. Timestamps are neglected   

#### Sequencer I/O specifications

**Output**
- Offers a Bluetooth service (NimBLE implementation)
- output MIDI data are encoded following the MIDI-BLE standard
- MIDI data are sent in order of playing time


**Input**
- can receive "real time" input over MQTT 
- later a connection to a musical instrument is envisioned

#### Sequencer mode of operation

- can accept real time input 
- can forward the real time input immediately to its output
- can store incoming data
- can simultaneously send stored data to it's output, e.g. in a loop

- data are stored in a cue with timestamped MIDI messages  
- for efficient processing or output, the way of storage (data encoding and storage structure) must enable sorting in order of time  
- timestamps in the cue must be suitable to span long times (minutes/hours). A 4 bytes timestamp that represents milliseconds or processor ticks (10 ms each) seems appropriate  
- for storage and handling have a look on various applications on the web (jdksmidi MIDI-Nimble ??)  




