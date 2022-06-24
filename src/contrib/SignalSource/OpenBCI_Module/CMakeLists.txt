###########################################################################
## Authors:
## Description: Build information for the OpenBCI_Module source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

bci2000_add_signal_source_module(
  OpenBCI_Module
  OpenBCI_ModuleADC.cpp
  SUPPORT batch/*=>/batch
  SUPPORT params/*=>/parms/fragments/amplifiers
)
