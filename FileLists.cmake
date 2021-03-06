SET(EXTH-DEMOS_HEADERS
    ${EXTH-DEMOS_SRC_DIR}/DemoChooserDialog/DemoChooserDialog.h)
    
SET(EXTH-DEMOS_SOURCES
    ${EXTH-DEMOS_SRC_DIR}/main.cpp
    ${EXTH-DEMOS_SRC_DIR}/DemoChooserDialog/DemoChooserDialog.cpp)

SET(EXTH-DEMOS_FORMS
    ${EXTH-DEMOS_SRC_DIR}/DemoChooserDialog/DemoChooserDialog.ui)

QT5_WRAP_UI(EXTH-DEMOS_FORM_INCLUDES ${EXTH-DEMOS_FORMS})


SET(EXTH-DEMOS_CONFIG_FILES
    ${EXTH-DEMOS_SRC_DIR}/CMakeLists.txt
    ${EXTH-DEMOS_SRC_DIR}/FileLists.cmake
    ${EXTH-DEMOS_SRC_DIR}/LibLists.cmake)

INCLUDE(Fluid2D/FileLists.cmake)
INCLUDE(Fractal/FileLists.cmake)
INCLUDE(Physics2D/FileLists.cmake)
INCLUDE(VolumeRendering/FileLists.cmake)

SET(EXTH-DEMOS_SRC_FILES
    ${EXTH-DEMOS_HEADERS}
    ${EXTH-DEMOS_SOURCES}
    ${EXTH-DEMOS_FORMS}
    ${EXTH-DEMOS_FORM_INCLUDES}
    ${EXTH-DEMOS_CONFIG_FILES}
    ${FLUID2D_SRC_FILES}
    ${FRACTAL_SRC_FILES}
    ${PHYSICS2D_SRC_FILES}
    ${VOLUME_RENDERING_SRC_FILES})


# Visual Studio filters
SOURCE_GROUP("Header Files" FILES ${EXTH-DEMOS_HEADERS})
SOURCE_GROUP("Source Files" FILES ${EXTH-DEMOS_SOURCES})
SOURCE_GROUP("Form Files" FILES ${EXTH-DEMOS_FORMS})
SOURCE_GROUP("Config" FILES ${EXTH-DEMOS_CONFIG_FILES})
