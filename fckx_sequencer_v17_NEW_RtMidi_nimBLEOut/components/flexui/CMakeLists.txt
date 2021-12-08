#The minimal component CMakeLists.txt file simply registers the component to the build #system using idf_component_register:

#idf_component_register(
#                      SRCS "foo.c" "bar.c"
#                       INCLUDE_DIRS "include"
#                       REQUIRES mbedtls
#)
#SRCS is a list of source files (*.c, *.cpp, *.cc, *.S). These source files will be #compiled into the component library.

#INCLUDE_DIRS is a list of directories to add to the global include search path for any #component which requires this component, and also the main source files.

#REQUIRES is not actually required, but it is very often required to declare what other #components this component will use. See Component Requirements.

#A library with the name of the component will be built and linked into the final app. #Directories are usually specified relative to the CMakeLists.txt file itself, although #they can be absolute.

#There are other arguments that can be passed to idf_component_register. These arguments #are discussed here.

#See example component requirements and example component CMakeLists for more complete #component CMakeLists.txt examples.
#-----------------------------------------------------------------------------


#set(EXTRA_COMPONENT_DIRS "../esp-nimble-ccp")  #FCKX


idf_component_register( SRCS    
                                "./src/flexui.cpp" 

                                

                       INCLUDE_DIRS 
				"./src" 
				"include" 


                      REQUIRES esp-nimble-cpp

)