project(opengl)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(NEED_GTEST TRUE)
SET(NEED_GLFW TRUE)
SET(NEED_ASSIMP TRUE)
SET(NEED_FREETYPE TRUE)
SET(NEED_STB_IMAGE TRUE)
SET(NEED_IRRKLANG TRUE)
SET(TARGET_NAME gl)
SET(TARGET_TYPE RUNTIME)
SET(NEED_COPY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resource/)
SET(AUTO_RUN ON)
SET(RUN_ARGS --gtest_filter=WOKUT.Hello)
# 开启生成compile_commands.json 用于代码分析
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

INCLUDE_DIRECTORIES(./src/practice/breakout/include)
# 手动包含标准库是为了在compile_commands.json中生成带有-I/stdlib的命令
include_directories(/Library/Developer/CommandLineTools/usr/include/c++/v1)

SOURCE_DIRS(./src
            ./src/object
            ./src/working
            ./src/practice/breakout
            ./src/practice/breakout/src
            #./src/practice
            #./src/advance_opengl
            #./src/advance_lighting
            #./src/getting_started
            #./src/lighting
            #./src/model
            )

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)
