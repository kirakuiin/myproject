MESSAGE(STATUS "freetype is used")
ADD_DEFINITIONS(-D__USING_FREETYPE__)

SET(FREETYPE_LIB freetype)

SET(LIB_FREETYPE_PATH ${MYDEPS}/freetype/lib/${MYPLATFORM})
seek_deps_library(${LIBS_PATH} ${LIB_FREETYPE_PATH} ${FREETYPE_LIB})

# 包含头文件路径和链接库
INCLUDE_DIRECTORIES(${MYDEPS}/freetype/include/freetype2/)
SET(LINK_FREETYPE_LIBS ${FREETYPE_LIB})
