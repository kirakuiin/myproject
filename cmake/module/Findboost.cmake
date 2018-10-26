MESSAGE(STATUS "boostlite is used")

# 包含头文件路径和链接库
INCLUDE_DIRECTORIES(${MYDEPS}/boost/include)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB -DBOOST_LIB_DIAGNOSTIC)

SET(BOOSTLITE_LIB boostlite)

SET(LIB_BOOST_PATH ${ABDEPS}/boost/lib/${ABPLATFORM}/${CMAKE_BUILD_TYPE})
seek_deps_library(${LIBS_PATH} ${LIB_BOOST_PATH} ${BOOSTLITE_LIB})

SET(LINK_BOOST_LIBS ${BOOSTLITE_LIB})

#gcc 4.8 or later disable -Wunused-local-typedefs
IF(REDHAT5)
    ADD_DEFINITIONS(-Wno-unused-local-typedefs)
ENDIF()

