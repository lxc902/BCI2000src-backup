###########################################################################
## $Id$
## Authors: mellinger@neurotechcenter.org

set (NAME WebcamRecorder)

utils_include(frameworks/Core)
bci2000_include(COM)
utils_set_appicon(video-camera)
bci2000_add_target(
    INFO Helper
    WINAPP ${NAME}
    WebcamRecorder.cpp
    GrabberTransform.cpp
    SaveToFileTransform.cpp
    WebcamPipeline.cpp
    VideoMediaType.cpp
    Window.cpp
    Application.cpp
    IniFile.cpp
)
if( NOT failed )
    target_link_libraries(${NAME} mf mfplat mfreadwrite mfuuid strmiids)
    set_property( TARGET ${NAME} PROPERTY FOLDER "Contrib/Extensions/WebcamLogger" )
    bci2000_add_to_inventory( Helper ${NAME} )
endif()

