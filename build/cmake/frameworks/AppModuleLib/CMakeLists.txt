###########################################################################
## $Id: CMakeLists.txt 6725 2022-05-06 14:08:30Z mellinger $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Builds a static library containing components specific to
##   application modules.

SET( NAME BCI2000FrameworkAppModule )

utils_include( frameworks/AppModuleLib/AppFiles )

# Add Extlib dependencies
IF( WIN32 )
  BCI2000_INCLUDE( SAPI )
ENDIF( WIN32 )
BCI2000_INCLUDE( 3DAPI )
BCI2000_INCLUDE( ffmpeg )
BCI2000_INCLUDE(Qt)
BCI2000_INCLUDE(PortAudio)

# Create the library project
UTILS_INCLUDE( frameworks/AppModule )
list( APPEND SRC_BCI2000_FRAMEWORK ${BCI2000_APP_FILES} )
list( APPEND LIBS ${BCI2000_APP_LIBS} )
BCI2000_ADD_REGISTRY( ${REGISTRY_NAME} SRC_BCI2000_FRAMEWORK DEPTARGETS )
BCI2000_ADD_TARGET( INFO "Framework library" STATIC_LIBRARY ${NAME} )

IF( FAILED )

  utils_warn(
    "Could not satisfy dependencies of the Application library.\n"
    "\tApplication modules will be omitted."
  )

ELSE( FAILED )

  TARGET_LINK_LIBRARIES( ${NAME} ${LIBS} )
  ADD_DEPENDENCIES( ${NAME} ${DEPTARGETS} )

ENDIF( FAILED )
