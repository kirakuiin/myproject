project(easy_engine)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(NEED_GLFW TRUE)
SET(NEED_BOOST TRUE)
SET(NEED_FREETYPE TRUE)
SET(NEED_STB_IMAGE TRUE)
SET(NEED_IRRKLANG TRUE)
SET(TARGET_NAME easy_engine)
SET(TARGET_TYPE LIBRARY)
SET(NEED_COPY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res/)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 手动包含标准库是为了在compile_commands.json中生成带有-I/stdlib的命令
include_directories(/Library/Developer/CommandLineTools/usr/include/c++/v1)

SOURCE_DIRS(
  ./src/widget
  ./src/utility
  ./src/opengl
  ./src/graphics
  ./src/physics
  )

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)

ADD_SUBDIRECTORY(test)
