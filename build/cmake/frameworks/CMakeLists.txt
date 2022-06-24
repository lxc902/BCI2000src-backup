###########################################################################
## $Id: CMakeLists.txt 6416 2021-11-04 15:40:04Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for framework libraries

SET( DIR_NAME "Core/Libraries/" )

ADD_SUBDIRECTORY( LibTiny )
ADD_SUBDIRECTORY( CoreLib )
ADD_SUBDIRECTORY( SigSrcModuleLib )
ADD_SUBDIRECTORY( SigProcModuleLib )
ADD_SUBDIRECTORY( AppModuleLib )
