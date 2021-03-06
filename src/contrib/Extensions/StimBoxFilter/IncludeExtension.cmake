###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com

# Add the StimBoxFilter to all application modules

if(WIN32)
SET( BCI2000_APP_FILES
   ${BCI2000_APP_FILES}
   ${BCI2000_EXTENSION_DIR}/StimBoxFilter.cpp
   ${BCI2000_EXTENSION_DIR}/lib/gSTIMbox.imports.cpp
)
else()
  message("gStimBoxFilter: Only available on Windows")
endif()
