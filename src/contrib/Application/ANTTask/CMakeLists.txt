###########################################################################
## Authors: schalklab@HR18818.wucon.wustl.edu
## Description: Build information for the ANTTask application module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( 3DAPI )

if(WIN32)

bci2000_add_application_module(
  ANTTask NOBUNDLE
  ANTTask.cpp
)

endif(WIN32)

