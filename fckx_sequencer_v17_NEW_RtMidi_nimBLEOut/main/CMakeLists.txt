set(COMPONENT_SRCS 
#NiCMidi files
#		"dump_tracks_dirty.cpp" 
#		"functions_dirty.cpp" "midi.cpp" "msg.cpp" "multitrack_dirty.cpp"
#		"RtMidi.cpp" "track_dirty.cpp" "sysex.cpp"
#other files
        "main.cpp"    #not required?
        "queue.cpp"   #not required?  
)
        
set(COMPONENT_ADD_INCLUDEDIRS "." )

register_component()