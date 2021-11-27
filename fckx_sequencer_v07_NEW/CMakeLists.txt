# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)



set(EXTRA_COMPONENT_DIRS $ENVC:/Users/Fred/esp_projects/MIDI-sequencer/fckx_sequencer_v07_NEW/components/NiCMidi/rtmidi-4.0.0)


#set(EXTRA_COMPONENT_DIRS $ENVC:/Users/Fred/esp_projects/MIDI-sequencer/fckx_sequencer_v07_NEW/components/NiCMidi)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(SUPPORTED_TARGETS esp32)
project(fckx_sequencer)
