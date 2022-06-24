###########################################################################
## $Id: FASTAPPROX.cmake 6416 2021-11-04 15:40:04Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including fastapprox in a project
## SETS:
##       SRC_EXTLIB - Required source files
##       HDR_EXTLIB - Required header files
##       INC_EXTLIB - Include directory

# Define the source files
SET( SRC_EXTLIB
)

# Define the headers
SET( HDR_EXTLIB
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/fastapprox/src )

# Set success
SET( EXTLIB_OK TRUE )
