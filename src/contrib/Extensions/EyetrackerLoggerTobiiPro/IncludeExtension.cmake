###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com

if(WIN32)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerTobiiPro.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/tobii_research.imports.cpp
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_research.dll 
	)
ELSE()
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x86/tobii_research.dll 
	)
ENDIF()

else()
  message("EyetrackerLoggerTobiiPro: Only available on Windows")
endif()
