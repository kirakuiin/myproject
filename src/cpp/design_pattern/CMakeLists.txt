project(design_pattern)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(NEED_GTEST TRUE)
SET(TARGET_NAME dput)
SET(TARGET_TYPE RUNTIME)
SET(AUTO_RUN ON)

SOURCE_DIRS(./src
            ./struct
            ./behave
            ./create)

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)
