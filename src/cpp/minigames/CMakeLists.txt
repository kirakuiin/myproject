project(minigames)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

# SET(NEED_GLFW TRUE)
SET(NEED_BOOST TRUE)

SET(TARGET_NAME minigames)
SET(TARGET_TYPE RUNTIME)
# SET(NEED_COPY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config/)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(AUTO_RUN ON)
# SET(RUN_ARGS --gtest_filter=WINUT.ButtonTest)

# 手动包含标准库是为了在compile_commands.json中生成带有-I/stdlib的命令
include_directories(
  /Library/Developer/CommandLineTools/usr/include/c++/v1
  ../easy_engine/include)

# 包含easy_engine动态库
SET(LINK_ALL_LIBS easy_engine)

SOURCE_DIRS()

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)
