###########################################################################
## $Id: LODEPNG.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the lodepng lib

SET( EXTLIB_OK TRUE )
SET( SRC_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp )
SET( HDR_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.h )
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/lodepng )
