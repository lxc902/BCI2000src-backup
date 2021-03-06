###########################################################################
## $Id: DELAUNATOR.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including delaunator in a project
## SETS:
##       SRC_EXTLIB - Required source files for the delaunator
##       HDR_EXTLIB - Required header files for the delaunator
##       INC_EXTLIB - Include directory for the delaunator

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/delaunator/include/delaunator.cpp
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/delaunator/include/delaunator.hpp
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/delaunator/include )

# Set success
SET( EXTLIB_OK TRUE )
