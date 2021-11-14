# MIDI-sequencer

MIDI sequencer/recorder functions are a too heavy load for the Faust DSP application on a TTGO TAudio V1.6 board. The DSP task is already pretty heavy in it's own, so it may be useful to dedicate the firmware of the audio board as much as possible to it's core task: **synthesising audio**. 

A solution is to add additional functionality to a second board, that does not need to have an audio codec. This second board can send MIDI commands to the audio board over a suitable interface.The received timestamped MIDI commands are delivered in the right order and can be played "immediately", without further processing except for using a queue and timers for respecting the synchronized MIDI timing.

The MIDI sequencer board can be connected with the synthesizer board over bluetooth. Additionally, the sequencer board may have a wired (UART) connection with a MIDI instrument, such as a keyboard.

## Implementation plan

1. Run a simple BT server/client on a ESP32 board that sends a stream of single notes
2. Connect a BT server/client on the TTGO TAudio board with the first board and demonstrate receipt of the data
3. Add the BT server/client to the Faust DSP application and demonstrate playing of the single notes
4. Add WIFI/MQTT functionality to the "sequencer" to communicate with Nodered in the same way as the Faust DSP app now does (version faust_mqtt_tcp6_nb_v5)
5. Demonstrate playing single notes entered via Nodered
6. Add more sequencing functionality, using e.g. the jdksmidi library
7. On success (matching the functionality of the current faust_mqtt_tcp6_nb_v5 firmware) remove the basic sequencing functionality from the DSP firmware


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
  - properly inclued the cpp_utils dependency (see [here](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils)
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

1. which role can push data to the other one? We do not want to let the Audio board do polling!  Read [this 1](https://embedded.fm/blog/ble-roles) and [this 2](https://web.archive.org/web/20160930015609/http://projects.mbientlab.com:80/bluetooth-low-energy-basics/) and specifically bullets 3 and 7 under data transfer.
2. how to show data transfer between server2 and client2?
3. look for the option that is lightest for the audio board.

Based on **this 2** a first guess for implementation of the MIDI synthesizer / sequencer application  would be to have the sequencer as the master  and server and the audio board as the slave/ client. Note: master and slave are roles during making of the connection and client/ server describe roles after the connection has been established.


