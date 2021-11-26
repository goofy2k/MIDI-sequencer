set(COMPONENT_SRCS 
                   	"advancedsequencer_dirty.cpp"
                   	"dump_tracks_dirty.cpp" 
                   	"functions_dirty.cpp" 
                   	"main.cpp" 
                   	"manager_dirty.cpp" 
		   	"matrix_dirty.cpp"
		   	"metronome_dirty.cpp"
                   	"midi.cpp" 
                   	"msg.cpp" 
                   	"multitrack_dirty.cpp" 
                   	"queue.cpp" 
#                  	"test_component.cpp" 
                   	"track_dirty.cpp"
                   	"sequencer_dirty.cpp" 
                   	"sysex.cpp" 
                   	"tick_dirty.cpp" 
                   	"timer.cpp"
                )
set(COMPONENT_ADD_INCLUDEDIRS 
			"." 
#			"../NicMidi/include" 
			"../include" 
		)

register_component()