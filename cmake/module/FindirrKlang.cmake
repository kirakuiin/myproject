MESSAGE(STATUS "irrKlang is used")
ADD_DEFINITIONS(-D__USING_IRRKLANG__)

# ����ͷ�ļ�·�������ӿ�
INCLUDE_DIRECTORIES(${MYDEPS}/irrklang/include)

SET(IRRKLANG_LIB irrklang)

SET(LIB_IRRKLANG_PATH ${MYDEPS}/irrklang/lib/${MYPLATFORM})
seek_deps_library(${LIBS_PATH} ${LIB_IRRKLANG_PATH} ${IRRKLANG_LIB})

SET(LINK_IRRKLANG_LIBS ${IRRKLANG_LIB})