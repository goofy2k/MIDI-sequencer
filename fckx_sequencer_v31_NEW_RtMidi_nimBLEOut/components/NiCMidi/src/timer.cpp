/*
 *   NiCMidi - A C++ Class Library for MIDI
 *
 *   Copyright (C) 2004  J.D. Koftinoff Software, Ltd.
 *   www.jdkoftinoff.com jeffk@jdkoftinoff.com
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


#include "../include/timer.h"

#include <iostream>         // for debugging

//#define ESP32

#ifdef ESP32
//#include "esp_log.h"        //FCKX!

#include <esp_pthread.h>

//#include "esp_pthread.h"

//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/pthread.html
esp_pthread_cfg_t MIDITimer::create_config(const char *name, int core_id, int stack, int prio)
{
    auto cfg = esp_pthread_get_default_config();
    cfg.thread_name = name;
    cfg.pin_to_core = core_id;
    cfg.stack_size = stack;
    cfg.prio = prio;
    return cfg;
}

#endif

const MIDITimer::timepoint MIDITimer::sys_clock_base = std::chrono::steady_clock::now();

unsigned int MIDITimer::resolution = MIDITimer::DEFAULT_RESOLUTION;

void* MIDITimer::tick_param = 0;
MIDITick* MIDITimer::tick_proc = 0;
std::atomic<int> MIDITimer::num_open(0);
MIDITimer::timepoint MIDITimer::current;
std::thread MIDITimer::bg_thread;


// We need this for assuring the destructor is called at exit, stopping the timer and joining the bg_thread
// Without this we have errors if exiting while the timer is open
static MIDITimer dummy;

MIDITimer::MIDITimer() {
    //std::cout << "MIDITimer constructor" << std::endl;
}

MIDITimer::~MIDITimer() {
    //std::cout << "MIDITimer destructor" << std::endl;
    HardStop();             // this joins the bg_thread
 };



void MIDITimer::SetResolution(unsigned int res) {
    int was_open = num_open;
    HardStop();
    resolution = res;
    if (was_open > 0) {
        Start();
        num_open = was_open;
    }
}


void MIDITimer::SetMIDITick(MIDITick t, void* tp) {
    int was_open = num_open;
    HardStop();
    tick_proc = t;
    tick_param = tp;
    if (was_open > 0) {
        Start();
        num_open = was_open;
    }
}


bool MIDITimer::Start () {
    //static const char *TAG = "MIDITimer::Start";
    //ESP_LOGE(TAG,"Enter with num_open %d", num_open);
    std::cout << "MIDITimer::Start enter with num_open: " << num_open << std::endl;

    if (tick_proc == 0)
        return false;                           // Callback not set

    num_open++;

    if (num_open == 1) {                         // Must create thread
        current = std::chrono::steady_clock::now();
       
        #ifdef ESP32
        // Create a thread using deafult values that can run on any core
        //auto cfg = esp_pthread_get_default_config();
       
        //Create a thread on core 1 that spawns another thread, they will both have the same name etc.
        //https://github.com/espressif/esp-idf/blob/7d43be967546f70b9229c2b5e1bd0ebd09392dfe/examples/cxx/pthread/main/cpp_pthread.cpp
        //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/pthread.html
        //esp_pthread_cfg_t MIDITimer::create_config(const char *name, int core_id, int stack, int prio)

        esp_pthread_cfg_t cfg = MIDITimer::create_config("ThreadProc", 1, 3 * 1024, 5);
        cfg.inherit_cfg = true;
        esp_pthread_set_cfg(&cfg);
        #endif
          
        bg_thread = std::thread(ThreadProc);
        std::cout << "Timer open with " << resolution << " msecs resolution" << std::endl;
    }
    //ESP_LOGE(TAG,"Exit with num_open %d", num_open);
    std::cout << "MIDITimer::Start exit with num_open: " << num_open << std::endl;
    return true;
}


void MIDITimer::Stop() {
    //static const char *TAG = "MIDITimer::Stop";
    //ESP_LOGE(TAG,"Enter with num_open %d", num_open);
    std::cout << "MIDITimer::Stop enter with num_open: " << num_open << std::endl;
    if (num_open > 0) {
        num_open--;
        if (num_open == 0) {  //FCKX! 0
            std:: cout << "MIDITimer::Stop num_open == 0 : GOING TO CALL bg_thread.join();" << std::endl;
            #ifdef ESP32
            //pthread_t res = pthread_join(bg_thread, NULL); //not valid
            bg_thread.join();
            #else
            bg_thread.join();
            #endif
            
            std:: cout << "Timer stopped by MIDITimer::Stop()" << std::endl;
        }
    }
    //ESP_LOGE(TAG,"Exit with num_open %d", num_open);
    std::cout << "MIDITimer::Stop exit with num_open: " << num_open << std::endl;
}


void MIDITimer::HardStop() {
    if (num_open > 0) {
        num_open = 0;
        bg_thread.join();
        std:: cout << "Timer stopped by MIDITimer::HardStop()" << std::endl;
    }
}

    // This is the background thread procedure
void MIDITimer::ThreadProc() {
    duration tick(resolution);
    std::cout << "before while block MIDITimer::ThreadProc() num_open: " << num_open << std::endl; //FCKX!
    while(MIDITimer::num_open) {
        // execute the supplied function
        // std::cout << "inside while block MIDITimer::ThreadProc() num_open: " << num_open << std::endl; //FCKX!
        MIDITimer::tick_proc(MIDITimer::GetSysTimeMs(), MIDITimer::tick_param);
        // find the next timepoint and sleep until it
        current += tick;
        std::this_thread::sleep_until(current);
    }
    std::cout << "after while block MIDITimer::ThreadProc() num_open: " << num_open << std::endl; //FCKX!

}


