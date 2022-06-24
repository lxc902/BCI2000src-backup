###########################################################################
## $Id: MATLAB_ENGINE.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including the matlab engine

set( matlabdir_ ${PROJECT_SRC_DIR}/extlib/matlab )
set( SRC_EXTLIB
    ${matlabdir_}/imports/libeng.imports.cpp
    ${matlabdir_}/imports/libmx.imports.cpp
)
set( HDR_EXTLIB
    ${matlabdir_}/engine.h
    ${matlabdir_}/mat.h
)
# use #include "matlab/engine.h" to avoid name clashes
set( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib )
set( EXTLIB_OK TRUE )

