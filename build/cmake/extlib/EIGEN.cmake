###########################################################################
## $Id: EIGEN.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including the Eigen lib
## SETS:
##       SRC_EXTLIB - Required source files for the Eigen library (none)
##       HDR_EXTLIB - Required header files for the Eigen library (none)
##       INC_EXTLIB - Include directory for the Eigen library

# Set success
SET( EXTLIB_OK TRUE )

# Define the source files
SET( SRC_EXTLIB
)
# Define the headers
SET( HDR_EXTLIB
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/eigen/eigen-3.3.7 )

