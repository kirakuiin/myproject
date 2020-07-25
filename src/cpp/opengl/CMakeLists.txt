project(opengl)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(NEED_GTEST TRUE)
SET(NEED_GLFW TRUE)
SET(NEED_ASSIMP TRUE)
SET(NEED_FREETYPE TRUE)
SET(NEED_STB_IMAGE TRUE)
SET(TARGET_NAME gl)
SET(TARGET_TYPE RUNTIME)
SET(NEED_COPY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resource/)
SET(AUTO_RUN ON)
SET(RUN_ARGS --gtest_filter=WOKUT.Hello)
SET(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

SOURCE_DIRS(./src
            ./src/object
            ./src/working
            #./src/practice
            #./src/advance_opengl
            #./src/advance_lighting
            #./src/getting_started
            #./src/lighting
            )

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)
