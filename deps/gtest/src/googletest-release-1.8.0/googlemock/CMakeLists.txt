PROJECT(googlegmock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

INCLUDE($ENV{MYCMAKE}/common/header.cmake)

SET(TARGET_NAME gmock)
SET(TARGET_TYPE ARCHIVE)

INCLUDE_DIRECTORIES(include
                    src
                    ../googletest/include)

SET(INCLUDE_SOURCE_FILES src/gmock-all.cc)

INCLUDE($ENV{MYCMAKE}/common/tailer.cmake)

