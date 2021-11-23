set(COMPONENT_SRCS "dump_tracks_dirty.cpp" 
                   "functions_dirty.cpp" 
                   "main.cpp" 
                   "manager_dirty.cpp" 
                   "midi.cpp" 
                   "msg.cpp" 
                   "multitrack_dirty.cpp" 
                   "queue.cpp" 
#                   "test_component.cpp" 
                   "track_dirty.cpp" 
                   "sysex.cpp" 
                   "tick.cpp" 
                   "timer.cpp"
                    )
set(COMPONENT_ADD_INCLUDEDIRS "." "../NicMidi/include" )

register_component()