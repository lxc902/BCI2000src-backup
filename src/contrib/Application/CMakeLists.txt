###########################################################################
## $Id: CMakeLists.txt 6711 2022-05-03 16:08:32Z lingling $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Application/" )

# Add this to the CMAKELists.txt in this directory
ADD_SUBDIRECTORY( ANTTask )


# Recurse down into all project subdirectories
IF( BUILD_MFC )
  UTILS_MATCH_SUBDIR( BUILD_MODULES MFCDemo )
ENDIF()
UTILS_MATCH_SUBDIR( BUILD_MODULES ERPSpeller )

