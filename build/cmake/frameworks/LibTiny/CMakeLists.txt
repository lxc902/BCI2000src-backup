###########################################################################
## $Id: CMakeLists.txt 6416 2021-11-04 15:40:04Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Defines a static library containing LibTiny utils.

UTILS_INCLUDE( frameworks/LibTiny/Files )

IF( WIN32 )
  SET( DEPLIBS ws2_32 winmm )
ENDIF() 
IF( NOT APPLE AND NOT WIN32 )
  SET( DEPLIBS rt pthread dl )
ENDIF()

add_subdirectory( StaticCrt )
add_subdirectory( DynamicCrt )
