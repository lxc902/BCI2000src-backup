###########################################################################
## $Id: SetOutputDirectoryMacro.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: A macro to set CMake output directories.
##   SET_OUTPUT_DIRECTORY( dir )

FUNCTION( SET_OUTPUT_DIRECTORY dir )

  GET_FILENAME_COMPONENT( dir_ ${dir} ABSOLUTE )
  SET( PROJECT_OUTPUT_DIR ${dir_} PARENT_SCOPE )
  
ENDFUNCTION()
