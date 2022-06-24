###########################################################################
## $Id: PORTAUDIO.cmake 6725 2022-05-06 14:08:30Z mellinger $
## Authors: mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including PortAudio in a project

set( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/portaudio/include" )
set( LIBS_EXTLIB )
set( HDR_EXTLIB )
set( SRC_EXTLIB )

if(WIN32)

  file( GLOB importfiles_ "${INC_EXTLIB}/../imports/*" )
  list( APPEND SRC_EXTLIB ${importfiles_} )
  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( libdir_ "${INC_EXTLIB}/../win32-amd64/dynamic" )
  else()
    set( libdir_ "${INC_EXTLIB}/../win32-x86/dynamic" )
  endif()
  file( GLOB dlls_ "${libdir_}/*.dll" )
  foreach( _R0 ${dlls_} )
    list( APPEND SRC_EXTLIB COMPANION ${_R0} )
  endforeach()
  set( EXTLIB_OK TRUE )

else()

  set( LIBS_EXTLIB -lportaudio )
  if( APPLE )
    set( LIBDIR_EXTLIB /opt/lib /usr/local/lib )
  endif()
  set( EXTLIB_OK true )

endif()

