project(minigames)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(NEED_GLFW TRUE)
SET(NEED_BOOST TRUE)
SET(NEED_EASYENGINE TRUE)

SET(TARGET_NAME minigames)
SET(TARGET_TYPE RUNTIME)
SET(NEED_COPY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resource)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(AUTO_RUN ON)

# 手动包含标准库是为了在compile_commands.json中生成带有-I/stdlib的命令
include_directories(/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/)

SOURCE_DIRS(
  src
  src/snake)

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)
