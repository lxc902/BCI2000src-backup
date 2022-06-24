###########################################################################
## $Id: IncludeExtension.cmake 5828 2018-11-28 16:45:06Z abelsten $
## Authors: jezhill@gmail.com

if( MSVC )

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/WebcamRecorder ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/contrib/Extensions/WebcamRecorder )

list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/WebcamLogger.cpp
   ${BCI2000_EXTENSION_DIR}/WebcamController.cpp
)

else( MSVC )

  utils_warn( "WebcamLogger: WebcamRecorder application only implemented on Windows" )

endif( MSVC )



