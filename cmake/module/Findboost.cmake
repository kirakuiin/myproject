MESSAGE(STATUS "boostlite is used")

# 包含头文件路径和链接库
INCLUDE_DIRECTORIES(${MYDEPS}/boost/include)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB -DBOOST_LIB_DIAGNOSTIC)

SET(BOOSTLITE_LIB boost_atomic
                  boost_chrono
                  boost_context
                  boost_coroutine
                  boost_date_time
                  boost_filesystem
                  boost_prg_exec_monitor
                  boost_program_options
                  boost_regex
                  boost_serialization
                  boost_system
                  boost_thread
                  boost_timer
                  boost_wserialization)

SET(LIB_BOOST_PATH ${MYDEPS}/boost/lib/${MYPLATFORM})
seek_deps_library(${LIBS_PATH} ${LIB_BOOST_PATH} ${BOOSTLITE_LIB})

SET(LINK_BOOST_LIBS ${BOOSTLITE_LIB})
