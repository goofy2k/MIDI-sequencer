set(COMPONENT_SRCS 
#NiCMidi files
#		"dump_tracks_dirty.cpp" 
#		"functions_dirty.cpp" "midi.cpp" "msg.cpp" "multitrack_dirty.cpp"
#		"track_dirty.cpp" "sysex.cpp"
#other files
        "main.cpp"  
        "queue.cpp"  )
        
set(COMPONENT_ADD_INCLUDEDIRS "." )

register_component()