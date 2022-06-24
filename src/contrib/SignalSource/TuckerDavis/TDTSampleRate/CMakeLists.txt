###########################################################################
## $Id: CMakeLists.txt 6668 2022-04-07 15:18:21Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the TDTSampleRate tool

# Set the executable name
set( NAME TDTSampleRate )

utils_include(frameworks/LibTiny)
utils_set_appicon(wrench)
set( SOURCES
  TDTSampleRate.cpp
  MainDialog.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
)

bci2000_add_target( INFO Tool QTAPP ${NAME} ${SOURCES} )
bci2000_add_to_inventory( Tool ${NAME} )
