###########################################################################
## $Id: IncludeExtension.cmake 6733 2022-05-10 15:15:31Z mellinger $
## Authors: griffin.milsap@gmail.com

SET( BCI2000_SIGSRC_FILES
  ${BCI2000_SIGSRC_FILES}
  ${BCI2000_SRC_DIR}/extlib/libsndfile/include/sndfile.h
  ${BCI2000_SRC_DIR}/extlib/libsndfile/imports/sndfile.imports.cpp
  ${BCI2000_SRC_DIR}/extlib/portaudio/include/portaudio.h
  ${BCI2000_SRC_DIR}/extlib/portaudio/imports/portaudio.imports.cpp
  ${BCI2000_EXTENSION_DIR}/AudioExtension.cpp
  ${BCI2000_SRC_DIR}/extlib/math/FilterDesign.cpp
)

if (WIN32)

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    list(APPEND BCI2000_SIGSRC_FILES COMPANION "${BCI2000_SRC_DIR}/extlib/libsndfile/win32-amd64/dynamic/sndfile64.dll")
    list(APPEND BCI2000_SIGSRC_FILES COMPANION "${BCI2000_SRC_DIR}/extlib/portaudio/win32-amd64/dynamic/portaudio_x64.dll")
  else()
    list(APPEND BCI2000_SIGSRC_FILES COMPANION "${BCI2000_SRC_DIR}/extlib/libsndfile/win32-x86/dynamic/sndfile32.dll")
    list(APPEND BCI2000_SIGSRC_FILES COMPANION "${BCI2000_SRC_DIR}/extlib/portaudio/win32-x86/dynamic/portaudio_x86.dll")
  endif()

else()

  list(APPEND BCI2000_SIGSRC_LIBS -lsndfile -lportaudio)

endif()
