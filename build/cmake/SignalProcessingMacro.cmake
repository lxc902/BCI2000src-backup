###########################################################################
## $Id: SignalProcessingMacro.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Contains a macro for creating a signal processing module

MACRO( BCI2000_ADD_SIGNAL_PROCESSING_MODULE )
  UTILS_PARSE_ARGS( "NAME;SOURCES" ${ARGV} )

  UTILS_INCLUDE( frameworks/SigProcModule )
  ADD_DEFINITIONS( -DMODTYPE=2 )
  utils_set_appicon( cogwheel-screen )
  BCI2000_ADD_CORE_MAIN( ${NAME} ${SOURCES} )
  BCI2000_ADD_TARGET( INFO "Signal processing module" EXECUTABLE ${NAME} ${SOURCES} )

  IF( NOT FAILED )
    BCI2000_ADD_TO_INVENTORY( SignalProcessing ${NAME} )
  ENDIF()

ENDMACRO()
