# MIDI-sequencer

Adding sequencer functions to the Faust DSP application on a TTGO TAudio V1.6 board appears to become a too heavy task.

A workaround for this is to add an interface to receive a stream of MIDI messages to the Faust DSP application. The received MIDI commands will be played "immediately", respecting the synchronized MIDI timing.

Recording of MIDI events, looping, editing etc. will be done in a MIDI sequencer application on a second board that does not have to run heavy DSP tasks. The MIDI sequencer board can be connected with the synthesizer board over bluetooth. Additionally, the sequencer board may contain a wired (UART) connection with a MIDI instrument, such as a keyboard.

## Implementation plan

1. Run a simple BT server/client on a ESP32 board that sends a stream of single notes
2. Connect a BT server/client on the TTGO TAudio board with the first board and demonstrate receipt of the data
3. Add the BT server/client to the Faust DSP application and demonstrate playing of the single notes
4. Add WIFI/MQTT functionality to the "sequencer" to communicate with Nodered in the same way as the Faust DSP app now does (version faust_mqtt_tcp6_nb_v5)
5. Demonstrate playing single notes entered via Nodered
6. Add more sequencing functionality, using e.g. the jdksmidi library
7. On success (matching the functionality of the current faust_mqtt_tcp6_nb_v5 firmware) remove the basic sequencing functionality from the DSP firmware


### REMARKS:

Scan [ESP-ADF](https://github.com/espressif/esp-adf) for this kind of applications.   NOT AVAILABLE

ESP-IDF and MIDI:

- [MIDIBLE](https://github.com/0x0c/MIDIBLE)

Arduino and MIDI:

- [ESP32-BLE-MIDI](https://www.arduino.cc/reference/en/libraries/esp32-ble-midi/) and the [repository](https://github.com/max22-/ESP32-BLE-MIDI)

Both examples (ESP-IDF and Arduino) are based on the [Arduino-BLE-MIDI](https://github.com/lathoub/Arduino-BLE-MIDI) lib
