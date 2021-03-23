MESSAGE(STATUS "\n**include header.cmake\n")

# 设置临时变量
MESSAGE(STATUS "\n**set variable----------------------------------------------")
SET (MYPROJECT $ENV{MYPROJECT} CACHE STRING "myproject path")
SET (MYCMAKE $ENV{MYCMAKE} CACHE STRING "my cmake file path")
SET (MYSRC $ENV{MYSRC} CACHE STRING "my source code path")
SET (MYDEPS $ENV{MYDEPS} CACHE STRING "my depend file path")
SET (MYTARGET $ENV{MYTARGET} CACHE STRING "my target file gen path")
MESSAGE(STATUS "SET MYPROJECT = ${MYPROJECT}")
MESSAGE(STATUS "SET MYCMAKE = ${MYCMAKE}")
MESSAGE(STATUS "SET MYSRC = ${MYSRC}")
MESSAGE(STATUS "SET MYDEPS = ${MYDEPS}")
MESSAGE(STATUS "SET MYTARGET = ${MYTARGET}")

# 设置生产包的安装路径
IF(NOT DEFINED PACKAGE_PATH)
    SET(PACKAGE_PATH ${MYPROJECT}/package CACHE STRING "package gene path")
    MESSAGE(STATUS "SET PACKAGE_PATH = ${PACKAGE_PATH}")
ENDIF()

# 设置安装路径
IF (NOT DEFINED INSTALL_PATH)
    SET(INSTALL_PATH .)
    MESSAGE(STATUS "SET INSTALL_PATH = ${INSTALL_PATH}")
ENDIF()

# 设置宽松if语句选项
# etc IF(A) ... ENDIF(A), 宽松后ENDIF()内不需要加A
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
MESSAGE(STATUS "SET CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS = ${CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS}")

# 设置cmake采用相对路径
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
MESSAGE(STATUS "SET CMAKE_USE_RELATIVE_PATHS = ${CMAKE_USE_RELATIVE_PATHS}")

# 将当前源文件目录自动添加到头文件引用路径下
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
MESSAGE(STATUS "SET CMAKE_INCLUDE_CURRENT_DIR = ${CMAKE_INCLUDE_CURRENT_DIR}")

# 输出详细编译信息
SET(CMAKE_VERBOSE_MAKEFILE ON)
MESSAGE(STATUS "SET CMAKE_VERBOSE_MAKEFILE = ${CMAKE_VERBOSE_MAKEFILE}")

# 设置cmake的模块引用路径
SET(CMAKE_MODULE_PATH ${MYCMAKE}/module)
MESSAGE(STATUS "SET CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

# 设置CMAKE_PLATFORM_VERSION
SET(CMAKE_PLATFORM_VERSION $ENV{CMAKE_PLATFORM_VERSION} CACHE STRING "platform version")

# 设置cmake构建类型
IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()
MESSAGE(STATUS "SET CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

# 设置cmake rpath选项
SET(CMAKE_INSTALL_RPATH .;..)
MESSAGE(STATUS "SET CMAKE_INSTALL_RPATH = ${CMAKE_INSTALL_RPATH}")

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
MESSAGE(STATUS "SET CMAKE_BUILD_WITH_INSTALL_RPATH = ${CMAKE_BUILD_WITH_INSTALL_RPATH}")

# 设置cmakeinstall 属性
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
MESSAGE(STATUS "SET CMAKE_INSTALL_RPATH_USE_LINK_PATH = ${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")

# 设置macosxrpath选项
SET(CMAKE_MACOSX_RPATH TRUE)
MESSAGE(STATUS "SET CMAKE_MACOSX_RPATH = ${CMAKE_MACOSX_RPATH}")

# 默认源文件目录，当前目录
SET(SOURCE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "the source file dirs")
MESSAGE(STATUS "SET SOURCE_DIRS = ${SOURCE_DIRS}")

# 检查编译版本 x86 and x64
MESSAGE(STATUS "GET CMAKE_BUILD_VERSION = ${CMAKE_BUILD_VERSION}")
IF("${CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "${CMAKE_BUILD_VERSION}" STREQUAL "x86" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "x86")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "i386" CACHE STRING "build version" FORCE)
ELSE()
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "x64" CACHE STRING "build version" FORCE)
ENDIF()
MESSAGE(STATUS "SET CMAKE_BUILD_VERSION = ${CMAKE_BUILD_VERSION}")

# 设置平台名称
SET(CMAKE_PLATFORM_NAME "$ENV{CMAKE_PLATFORM_NAME}" CACHE STRING "platform name")
MESSAGE(STATUS "SET CMAKE_PLATFORM_NAME = ${CMAKE_PLATFORM_NAME}")

IF("${CMAKE_PLATFORM_NAME}" STREQUAL "Redhat")
    SET(REDHAT5 ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "Darwin")
    SET(DARWIN ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "Linux")
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "el7")
        SET(el7 ON)
    ENDIF()
ENDIF()

# 设置OS类型
MESSAGE(STATUS "GET CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF(WIN32)
    SET(OS_WINDOWS ON)
    MESSAGE(STATUS "SET OS_WINDOWS = ${OS_WINDOWS}")
ELSEIF(DARWIN)
    SET(OS_DARWIN ON)
    MESSAGE(STATUS "SET OS_DARWIN = ${OS_DARWIN}")
ELSE()
    SET(OS_LINUX ON)
    MESSAGE(STATUS "SET OS_LINUX = ${OS_LINUX}")
ENDIF()

# 设置MYPLATFORM
IF(DARWIN)
    SET(MYPLATFORM ${CMAKE_PLATFORM_NAME}_${CMAKE_BUILD_VERSION} CACHE STRING "platform full name")
ELSE()
    SET(MYPLATFORM ${CMAKE_PLATFORM_NAME}_${CMAKE_PLATFORM_VERSION}_${CMAKE_BUILD_VERSION} CACHE STRING "platform full name")
ENDIF()
MESSAGE(STATUS "SET MYPLATFORM = ${MYPLATFORM}")

# 设置LIBS_PATH
IF(NOT DEFINED LIBS_PATH)
    SET(LIBS_PATH ${MYTARGET}/${CMAKE_BUILD_TYPE})
    MESSAGE(STATUS "SET LIBS_PATH = ${LIBS_PATH}")
ENDIF()

# 设置默认的打包路径
SET(INSTALL_SUB_PATH ".")
MESSAGE(STATUS "SET INSTALL_SUB_PATH = ${INSTALL_SUB_PATH}")


# 宏定义
MESSAGE(STATUS "\n**define----------------------------------------------")

IF(WIN32)
    # /Zc:wchar_t 这个参数导致我们所有用到wchar_t的库编译都需要加这个
    ADD_DEFINITIONS(/D _UNICODE
                    /D UNICODE
                    /D WIN32
                    /D __WINDOWS__
                    /D _CRT_SECURE_NO_WARNINGS
                    /D _CRT_SECURE_NO_DEPRECATE
                    # The following POSIX name for functions are deprecated
                    /D _CRT_NONSTDC_NO_DEPRECATE
        )
ELSEIF(REDHAT5 OR el7)
    ADD_DEFINITIONS(-D__LINUX__ -D_GNU_SOURCE)
    IF("$ENV{CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "${CMAKE_BUILD_VERSION}" STREQUAL "i386")
        ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=63 -D_LARGEFILE64_SOURCE=1)
    ENDIF()
ELSEIF(DARWIN)
    ADD_DEFINITIONS(-D_GNU_SOURCE)
ENDIF()

IF(NOT NO_TESTENDIAN)
    IF(WIN32)
        ADD_DEFINITIONS(-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__)
        MESSAGE(STATUS "-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__")
    ELSE()
        INCLUDE(TestBigEndian)
        TEST_BIG_ENDIAN(TEST_ENDIAN)
        IF(TEST_ENDIAN)
            ADD_DEFINITIONS(-D__BIG_ENDIAN__ -D__IS_BIG_ENDIAN__)
            MESSAGE(STATUS "-D__BIG_ENDIAN__ -D__IS_BIG_ENDIAN__")
        ELSE()
            ADD_DEFINITIONS(-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__)
            MESSAGE(STATUS "-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__")
        ENDIF()
    ENDIF()
    SET(NO_TESTENDIAN TRUE)
ENDIF()
MESSAGE(STATUS "NO_TESTENDIAN = ${NO_TESTENDIAN}")

MESSAGE(STATUS "\n**funs_macros----------------------------------------------")
# 功能函数以及功能宏定义

##定义宏来排除不需要编译的文件
MACRO(FILTER_OUT FILTERS INPUTS OUTPUTS)
    SET(FOUT "")
    FOREACH(INP ${INPUTS})
        SET(FILTERED 0)
        FOREACH(FILT ${FILTERS})
            IF(${FILTERED} EQUAL 0)
                IF("${FILT}" STREQUAL "${INP}")
                    SET(FILTERED 1)
                ENDIF()
            ENDIF()
        ENDFOREACH()
        IF(${FILTERED} EQUAL 0)
            SET(FOUT ${FOUT} ${INP})
        ENDIF()
    ENDFOREACH()
    SET(${OUTPUTS} ${FOUT})
ENDMACRO()
MESSAGE(STATUS "MACRO(FILTER_OUT FILTERS INPUTS OUTPUS)")

##时间宏获得当前时间
MACRO(TODAY RESULT)
    IF(WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C echo $DATE:~0,4%-%DATE:~5,2%-%DATE:~8,2%" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/(..).*" "\\1\\2\\3" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/(..).*" "\\1\\2\\3" ${RESULT} ${${RESULT}})
    ELSE(WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
    ENDIF()
ENDMACRO()
MESSAGE(STATUS "MACRO(TODAY RESULT)")

# 添加前缀宏用来给变量添加前缀
MACRO(ADD_PREFIX ORGN _PREFIX DEST)
    SET(_DEST "")
    FOREACH(_EACH_ORGN ${ORGN})
        IF("${_EACH_ORGN}" STREQUAL "")
            MESSAGE(WARNING "${_EACH_ORGN} IS NULL")
        ELSE()
            SET(_EACH_ORGN ${_PREFIX}${_EACH_ORGN})
            SET(_DEST ${_DEST} ${_EACH_ORGN})
        ENDIF()
    ENDFOREACH()
    SET(${DEST} ${_DEST})
ENDMACRO()
MESSAGE(STATUS "MACRO(ADD_PREFIX ORGN _PREFIX DEST)")

# 将po文件翻译为mo文件并放到指定目录
MACRO(ADD_RES_FILES_TARGET SOUR DEST CLEAN_FILES)
    IF(WIN32)
        SET(MSGFMT ${MYCMAKE}/tool/win/bin/msgfmt.exe)
    ELSE()
        SET(MSGFMT msgfmt)
    ENDIF()

    SET(LANG_DATA_PATH "locale")
    SET(LANG_TYPE "en_Us" "zh_CN")

    FOREACH(THE_LANG_TYPE ${LANG_TYPE})
        SET(IN_LANG_REAL_PATH ${SOUR}/${THE_LANG_TYPE})
        SET(OUT_LANG_REAL_PATH ${DEST}/${LANG_DATA_PATH}/${THE_LANG_TYPE})
        SET(INSTALL_LANG_PATH ${LANG_DATA_PATH}/${THE_LANG_TYPE})

        # get po file list
        FILE(GLOB PO_FILE_LIST "${IN_LANG_REAL_PATH}/*.po")
        FOREACH(THE_PO_FILE ${PO_FILE_LIST})
            GET_FILENAME_COMPONENT(THE_PO_FILE_NAME ${THE_PO_FILE} NAME_WE)
            IF(EXISTS ${OUT_LANG_REAL_PATH}/${THE_PO_FILE_NAME}.mo)
                FILE(REMOVE_RECURSE ${OUT_LANG_REAL_PATH}/${THE_PO_FILE_NAME}.mo)
            ENDIF()
            IF(NOT EXISTS ${OUT_LANG_REAL_PATH})
                FILE(MAKE_DIRECTORY ${OUT_LANG_REAL_PATH})
            ENDIF()
            MESSAGE(STATUS "${THE_PO_FILE} -o ${OUT_LANG_REAL_PATH}/${THE_PO_FILE_NAME}.mo")
            EXECUTE_PROCESS(COMMAND ${MSGFMT} ${THE_PO_FILE} -o ${OUT_LANG_REAL_PATH}/${THE_PO_FILE_NAME}.mo WORKING_DIRECTORY ${IN_LANG_REAL_PATH} RESULT_VARIABLE RES_VAR ERROR_VARIABLE RES_ERROR)

            IF(${RES_VAR})
                MESSAGE("")
                MESSAGE(STATUS "FATAL_ERROR:${RES_ERROR}")
                MESSAGE(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) ${THE_PO_FILE} FAILED")
            ENDIF()
            LIST(APPEND CLEAN_FILES ${OUT_LANG_REAL_PATH}/${THE_PO_FILE_NAME}.mo)
        ENDFOREACH()
    ENDFOREACH()
ENDMACRO()
MESSAGE(STATUS "MACRO(TRANSLATE_RES_FILES SOUR DEST CLEAN_FILES)")

# 将指定文件拷贝到安装路径
MACRO(ADD_FILE_TARGET TARGETPATH FILEPATH FILENAME)
    IF(NOT EXISTS ${LIBS_PATH}/${TARGETPATH})
        FILE(MAKE_DIRECTORY ${LIBS_PATH}/${TARGETPATH})
    ENDIF()
    FILE(INSTALL ${FILEPATH}/${FILENAME} DESTINATION ${LIBS_PATH}/${TARGETPATH} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
    # 这句话好像和上面的话重复了
    INSTALL_FILES(${TARGETPATH} ${FILEPATH}/${FILENAME})
ENDMACRO()
MESSAGE(STATUS "MACRO(ADD_FILE_TARGET TARGETPATH FILEPATH FILENAME)")

# 测试函数的参数
function(test_func a b)
MESSAGE(STATUS "\n**test_func----------------------------------------------")
    MESSAGE(STATUS "argc=${ARGC}")
    MESSAGE(STATUS "argn=${ARGN}")
    # 函数支持${ARGV}即额外参数列表，而宏不支持
    MESSAGE(STATUS "argv=${ARGV}")
    MESSAGE(STATUS "argv0=${ARGV0}")
    MESSAGE(STATUS "argv1=${ARGV1}")
    MESSAGE(STATUS "argv2=${ARGV2}")
    MESSAGE(STATUS "a=${a}")
    MESSAGE(STATUS "b=${b}")
    # 函数支持上层定义域
    SET(${a} ${${b}} PARENT_SCOPE)
    SET(_MSG_VER 1400)
    MESSAGE(STATUS "SET _MSG_VER = ${_MSG_VER}")
    SET(TEST_VER "_MSG_VER > 1900")
    MESSAGE(STATUS "SET TEST_VER = ${TEST_VER}")
    IF(TEST_VER)
        MESSAGE("WTF")
    ELSE()
        MESSAGE("OOPS")
    ENDIF()
MESSAGE(STATUS "\n**test_end----------------------------------------------")
endfunction()

# 支持可变参数 将参数路径添加到源代码路径
function(source_dirs)
    IF(${ARGC} LESS 1)
        RETURN()
    ELSE()
        FOREACH(EACH_SOURCE_DIR ${ARGN})
            IF("${EACH_SOURCE_DIR}" STREQUAL ".")
                # 当前路径已添加
            ELSEIF(IS_ABSOLUTE ${EACH_SOURCE_DIR})
                LIST(APPEND _SOURCE_DIRS ${EACH_SOURCE_DIR})
            ELSE()
                LIST(APPEND _SOURCE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${EACH_SOURCE_DIR})
            ENDIF()
        ENDFOREACH()
        SET(SOURCE_DIRS ${SOURCE_DIRS} ${_SOURCE_DIRS} PARENT_SCOPE)
    ENDIF()
endfunction()
MESSAGE(STATUS "function(source_dirs)")

# 支持可变参数列表 将参数路径加入到安装路径
function(add_install_dirs)
    IF(${ARGC} LESS 1)
        RETURN()
    ELSE()
        FOREACH(EACH_INSTALL_DIR ${ARGN})
            IF(IS_ABSOLUTE ${EACH_INSTALL_DIR})
                MESSAGE(FATAL_ERROR "CAN NOT SPECIFY ABSOLUTE PATHS")
            ELSE()
                LIST(APPEND _INSTALL_DIRS ${EACH_INSTALL_DIR})
            ENDIF()
        ENDFOREACH()
        SET(INSTALL_SUB_PATH ${INSTALL_SUB_PATH} ${_INSTALL_DIRS} PARENT_SCOPE)
    ENDIF()
endfunction()
MESSAGE(STATUS "function(add_install_dirs)")

# 将源目录下的某个文件拷贝到所有安装路径下的dest当中
function(install_file source dest)
    IF(${ARGC} LESS 1)
        MESSAGE(FATAL_ERROR "install_file DON'T SUPPORT ARGUMENTS LESS THEN 2")
    ENDIF()

    IF(DEFINED dest AND IS_ABSOLUTE ${dest})
        MESSAGE(FATAL_ERROR "install_file DON'T SUPPORT ABSOLUTE PATH")
    ENDIF()

    FOREACH(SUBDIR ${INSTALL_SUB_PATH})
        IF(NOT "${SUBDIR}" STREQUAL ".")
            SET(TARGET ${INSTALL_PATH}/${SUBDIR})
        ELSE()
            SET(TARGET ${INSTALL_PATH})
        ENDIF()

        IF(NOT "${DEST}" STREQUAL "." AND NOT "${DEST}" STREQUAL "")
            SET(TARGET ${TARGET}/${DEST})
        ENDIF()

        INSTALL(FILES ${source} DESTINATION ${TARGET} PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

    ENDFOREACH()
endfunction()
MESSAGE(STATUS "function(install_file source dest)")

# 将source之中的文件拷贝到dest目录
function(install_files dest source)
    LIST(LENGTH ARGV ARGV_LEN)
    SET(I 1)
    # source 感觉没有存在的必要，既然都用的是参数列表了
    WHILE(I LESS ${ARGV_LEN})
        LIST(GET ARGV ${I} ARGV_VALUE)
        install_file(${ARGV_VALUE} ${dest})
        MATH(EXPR I "${I} + 1")
    ENDWHILE()
endfunction()
MESSAGE(STATUS "function(install_files dest source)")

# 将指定目录里的文件拷贝到指定目录中，并排除中间文件
function(install_dir source dest)
    if(${ARGC} LESS 2)
        MESSAGE(FATAL_ERROR "install_dir DON'T SUPPORT ARGUMENTS LESS THAN 2!")
    ENDIF()

    if(DEFINED dest AND IS_ABSOLUTE ${dest})
        MESSAGE(FATAL_ERROR "install_dir DON'T SUPPORT ABSOLUTE PATH!")
    ENDIF()

    FOREACH(SUBDIR ${INSTALL_SUB_PATH})
        MESSAGE(STATUS "SUBDIR=${SUBDIR}")
        IF(NOT "${SUBDIR}" STREQUAL ".")
            SET(TARGET ${INSTALL_PATH}/${SUBDIR})
        ELSE()
            SET(TARGET ${INSTALL_PATH})
        ENDIF()

        IF(NOT "${dest}" STREQUAL "." AND NOT "${dest}" STREQUAL "")
            SET(TARGET ${TARGET}/${dest})
        ENDIF()
        MESSAGE( "SOURCE = ${source}" )
        MESSAGE( "TARGET = ${TARGET}" )
        MESSAGE( "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

        INSTALL(DIRECTORY ${source} DESTINATION ${TARGET}
            PATTERN "*.lib" EXCLUDE
            PATTERN "*.def" EXCLUDE
            PATTERN "*.a" EXCLUDE
            PATTERN "*.po" EXCLUDE
            PATTERN "*.sql" EXCLUDE
            PATTERN "*.dll.manifest" EXCLUDE
            PATTERN "*.CMakeLists" EXCLUDE
            PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
            GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
    ENDFOREACH()
endfunction()
MESSAGE(STATUS "function(install_dir source dest)")

# 将源目录下的所有文件拷贝到所有安装路径下的dest当中
function(install_dirs dest source)
    LIST(LENGTH ARGV ARGV_LEN)
    SET(I 1)
    WHILE( I LESS ${ARGV_LEN})
        LIST(GET ARGV ${I} ARGV_VALUE)
        install_dir(${ARGV_VALUE} ${DEST})
        MATH(EXPR I "${I} + 1")
    ENDWHILE()
endfunction()
MESSAGE(STATUS "function(install_dirs dest source)")

# 将目标文件安装到指定目录
function(install_target type target_name dest)
    IF(${ARGC} LESS 3)
        MESSAGE(FATAL_ERROR "install_target DON'T SUPPORT ARGUMENTS LESS THEN 3")
    ENDIF()

    IF(DEFINED dest AND IS_ABSOLUTE ${dest})
        MESSAGE(FATAL_ERROR "install_target DON'T SUPPORT ABSOLUTE PATH")
    ENDIF()

    FOREACH(SUBDIR ${INSTALL_SUB_PATH})
        IF(NOT "${SUBDIR}" STREQUAL ".")
            SET(TARGET ${INSTALL_PATH}/${SUBDIR})
        ELSE()
            SET(TARGET ${INSTALL_PATH})
        ENDIF()

        IF(NOT "${dest}" STREQUAL "." AND NOT "${dest}" STREQUAL "")
            SET(TARGET ${TARGET}/${dest})
        ENDIF()

        INSTALL(TARGETS ${target_name} ${type}
            DESTINATION ${TARGET}
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ)
        IF(WIN32)
            INSTALL(FILES ${LIBS_PATH}/${dest}/${target_name}.pdb
                DESTINATION ${TARGET}
                PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
                GROUP_EXECUTE GROUP_READ
                WORLD_EXECUTE WORLD_READ)
        ENDIF()
    ENDFOREACH()
endfunction()
MESSAGE(STATUS "function(install_target type target_name dest)")

#
## 以下四个seek函数，path1为outputpath path2为库源目录
#

# 在两个目录中搜索可变参数指定的库
function(seek_deps_library_no_install path1 path2)
    IF(${ARGC} LESS 3)
        MESSAGE(FATAL_ERROR "install_target DON'T SUPPORT ARGUMENTS LESS THEN 3")
    ENDIF()

    FIND_LIBRARY(FIRST_LIB NAMES ${ARGN} PATHS ${path1} NO_DEFAULT_PATH)
    IF("${FIRST_LIB}" STREQUAL "FIRST_LIB-NOTFOUND")
        FIND_LIBRARY(SECOND_LIB NAMES ${ARGN} PATHS ${path2} NO_DEFAULT_PATH)
        IF("${SECOND_LIB}" STREQUAL "SECOND_LIB-NOTFOUND")
            MESSAGE(FATAL_ERROR "CAN'T FOUND LIB ${ARGN}")
        ELSE()
            # 将path2的库拷贝到path1之中
            FILE(INSTALL ${path2}/ DESTINATION ${path1}
                FILE_PERMISSIONS OWNER_WRITE OWNER_EXECUTE OWNER_READ
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE)
        ENDIF()
        # 清除缓存以便下次重新查找
        UNSET(SECOND_LIB CACHE)
    ENDIF()
    # 清除缓存以便下次重新查找
    UNSET(FIRST_LIB CACHE)
endfunction()
MESSAGE(STATUS "function(seek_deps_library_no_install path1 path2)")

# 寻找库是否在path1或者path2中
function(seek_deps_library path1 path2)
    MESSAGE(STATUS "path1=${path1} path2=${path2}")
    seek_deps_library_no_install(${path1} ${path2} ${ARGN})
    # 将源目录文件拷贝到当前目录
    IF(ENABLE_PACKAGE)
        install_dir (${path2}/ .)
    ENDIF()
endfunction()
MESSAGE(STATUS "function(seek_deps_library path1 path2)")

#搜索指定文件，并将在path2之中找到的复制到path1之中
function(seek_deps_file_no_install path1 path2)
    IF(${ARGC} LESS 3)
        MESSAGE(FATAL_ERROR "PARAMETER ERROR, SEEK_DEPS_FILE_NO_INSTALL NEED THREE OR MORE PARAMETERS.")
    ENDIF()

    # 第一次从${path1}查找
    FIND_FILE(FIRST_FILE NAMES ${ARGN} PATHS ${path1} NO_DEFAULT_PATH)
    IF("${FIRST_FILE}" STREQUAL "FIRST_FILE-NOTFOUND")

        # 第二次从${path2}查找
        FIND_FILE(SECOND_FILE NAMES ${ARGN} PATHS ${path2} NO_DEFAULT_PATH)
        IF("${SECOND_FILE}" STREQUAL "SECOND_FILE-NOTFOUND")
            MESSAGE(FATAL_ERROR "CAN'T FOUND FILE ${ARGN}")
        ELSE()
            FILE(INSTALL ${path2}/ DESTINATION ${path1}
                FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                )
        ENDIF()
        # 清除缓存以便下次重新查找
        UNSET(SECOND_FILE CACHE)
    ENDIF()
    # 清除缓存以便下次重新查找
    unset(FIRST_FILE CACHE)
endfunction()
MESSAGE(STATUS "function(seek_deps_file_no_install path1 path2)")

# 寻找某个文件是否存在
function(seek_deps_file path1 path2)
    seek_deps_file_no_install(${path1} ${path2} ${ARGN})
    IF(ENABLE_PACKAGE)
        install_dir(${path2}/ .)
    ENDIF()
endfunction()
MESSAGE(STATUS "function(seek_deps_file path1 path2)")

# 将某个路径下的thrift文件全部转化为cpp/py/php文件
function(convert_thrift_by_path module thrift_file_path out_path)
    SET(THRIFT ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)

    IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${out_path})
        FILE(MAKE_DIRECTORY ${out_path})
    ENDIF()

    IF(DEFINED THRIFT_DEPS_PATHS)
        FOREACH(EACH_DEPS_PATH ${THRIFT_DEPS_PATHS})
            LIST(APPEND DEPS_THRIFT_FLAGS -I ${EACH_DEPS_PATH})
        ENDFOREACH()
    ENDIF()

    FILE(GLOB THRIFT_FILE_LIST "${thrift_file_path}/*.thrift")

    MESSAGE(STATUS "CMD: ${THRIFT} -v -r ${DEPS_THRIFT_FLAGS}
    --gen ${module}
    --out ${out_path} ${thrift_file}")

    FOREACH(THRIFT_FILE ${THRIFT_FILE_LIST})
        EXECUTE_PROCESS(COMMAND ${THRIFT} -v -r ${DEPS_THRIFT_FLAGS}
            --gen ${module}
            --out ${out_path} ${thrift_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE RET)
        IF(${RET} EQUAL 0)
            MESSAGE(STATUS "CONVERT ${THRIFT_FILE} SUCCESS.")
        ELSE()
            MESSAGE(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) CONVERT ${THRIFT_FILE} FAILED!!!")
        ENDIF()
    ENDFOREACH()

    SET(ADDITIONAL_CLEAN_FILES ${ADDITIONAL_CLEAN_FILES};${CMAKE_CURRENT_SOURCE_DIR}/${out_path} PARENT_SCOPE)
endfunction()
MESSAGE(STATUS "function(convert_thrift_by_path module thrift_file_path out_path)")

# 编译thrift文件(cpp)
MESSAGE(STATUS "\n**compile_thrift----------------------------------------------")

function(compile_thrift thrift_files thrift_out_dir deps_thrift_path srcs hdrs)
    FILE(MAKE_DIRECTORY ${thrift_out_dir})
    SET(THRIFT_DIR ${thrift_out_dir}/gen-cpp)

    IF(WIN32)
        SET(THRIFTCMD ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift.exe)
    ELSE()
        SET(THRIFTCMD ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)
        EXECUTE_PROCESS(COMMAND chmod +x ${THRIFTCMD})
    ENDIF()

    MESSAGE(STATUS "compiling thrift files...")

    SET(${srcs})
    SET(${hdrs})

    FOREACH(THRIFT_FILE ${thrift_files})
        GET_FILENAME_COMPONENT(ABS_FIL ${THRIFT_FILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(FIL_WE ${THRIFT_FILE} NAME_WE)
        LIST(APPEND ${hdrs} "${thrift_out_dir}/gen-cpp/${FIL_WE}.h")
        LIST(APPEND ${srcs} "${thrift_out_dir}/gen-cpp/${FIL_EW}.cpp")

        LIST(APPEND ${CLEAN_THRIFT_FILES} "${thrift_out_dir}/gen-cpp/${FIL_WE}.h")
        LIST(APPEND ${CLEAN_THRIFT_FILES} "${thrift_out_dir}/gen-cpp/${FIL_EW}.cpp")

        # 所有thrift都依赖basedatatype，添加到默认查找路径里来
        ADD_CUSTOM_COMMAND(OUTPUT "${thrift_out_dir}/gen-cpp/${FIL_EW}.h" "${thrift_out_dir}/gen-cpp/${FIL_EW}.cpp"
            COMMAND ${THRIFTCMD}
            ARGS -o ${thrift_out_dir}
            -I ${deps_thrift_path}
            -I $ENV{BASETHRIFTDTS}
            --gen cpp ${THRIFT_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "")

    ENDFOREACH()

    MESSAGE(STATUS "COMPILING THRIFT FILES SUCCESS ...")

    INCLUDE_DIRECTORIES(${thrift_out_dir})
    INCLUDE_DIRECTORIES(${THRIFT_DIR})
    SET_SOURCE_FILES_PROPERTIES(${${srcs}} ${${hdrs}} PROPERTIES GENERATED TRUE)
    # 可以参照指针的概念进行理解
    SET(${srcs} ${${srcs}} PARENT_SCOPE)
    SET(${hdrs} ${${hdrs}} PARENT_SCOPE)

endfunction()
MESSAGE(STATUS "function(compile_thrift thrift_files thrift_out_dir deps_thrift_path srcs hdrs)")

## 编译thrift文件
MACRO(ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR GEN_DIR_NAME DEPS_THRIFT_PATH)
    FOREACH(THRIFT_FILE ${ARGN})
        IF(NOT DEFINED DEPS_THRIFT_PATH)
            SET(DEPS_THRIFT_PATH ".")
        ENDIF()
        compile_thrift(${THRIFT_FILE} ${PROJECT_BINARY_DIR}/${GEN_DIR_NAME}
            ${DEPS_THRIFT_PATH} CUR_THRIFT_GEN_CPP_SRCS CUR_THRIFT_GEN_CPP_HDRS)
        SET(THRIFT_GEN_CPP_SRCS ${THRIFT_GEN_CPP_SRCS} ${CUR_THRIFT_GEN_CPP_SRCS})
        SET(THRIFT_GEN_CPP_HDRS ${THRIFT_GEN_CPP_HDRS} ${CUR_THRIFT_GEN_CPP_HDRS})
    ENDFOREACH()
    MESSAGE(STATUS "ADDING THRIFT FILES DIR ${PROJECT_BINARY_DIR}/${GEN_DIR_NAME}/gen-cpp")
ENDMACRO()
MESSAGE(STATUS "MARCO(ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR GEN_DIR_NAME DEPS_THRIFT_PATH)")

# 构建编译thrift文件
MACRO(ADD_BUILD_THRIFT_FILES)
    ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR(project_thrift . ${ARGN})
ENDMACRO()
MESSAGE(STATUS "MACRO(ADD_BUILD_THRIFT_FILES)")

#
## 开关选项
#

MESSAGE(STATUS "\n**option----------------------------------------------")

# 设置 make package的选项 默认on
IF(NOT DEFINED ENABLE_PACKAGE)
    MESSAGE(STATUS "GET ENABLE_PACKAGE = ${ENABLE_PACKAGE}")
    OPTION(ENABLE_PACKAGE "enable make package to install libs" ON)
ENDIF()
MESSAGE(STATUS "GET ENABLE_PACKAGE = ${ENABLE_PACKAGE}")

IF(NOT DEFINED PACKAGE_PATH)
    SET(PACKAGE_PATH ${MYCMAKE}/packages CACHE STRING "package path")
    MESSAGE(STATUS "SET PACKAGE_PATH = ${PACKAGE_PATH}")
ENDIF()

# 设置可执行文件是否自动执行 默认off
IF(NOT DEFINED AUTO_RUN)
    OPTION(AUTO_RUN "auto running executable file" OFF)
ENDIF()
MESSAGE(STATUS "GET AUTO_RUN = ${AUTO_RUN}")

# 是否支持交叉编译 默认off
IF(NOT DEFINED XCOMPILE)
    OPTION(XCOMPILE "using cross platform compilation" OFF)
ELSE()
    SET(ENV{LD_LIBRARY_PATH} /usr/local/lib:$ENV{LD_LIBRARY_PATH})
    MESSAGE(STATUS "SET LD_LIBRARY_PATH = ENV{LD_LIBRARY_PATH}")
ENDIF()
MESSAGE(STATUS "GET XCOMPILE = ${XCOMPILE}")

MESSAGE(STATUS "\n**check----------------------------------------------")

IF(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /vmm /vmg /MP /wo4067 /wo4267 /wo4181 /wd4099")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /INCREMENTAL:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

    MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS_RELEASE = ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
    MESSAGE(STATUS "SET CMAKE_EXE_LINKER_FLAGS_RELEASE = ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

    STRING(RELPACE "/EHsc" "/EHA /GS" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
ELSEIF(DARWIN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -pipe -O0 -Wall -pthread")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -pipe -O0 -Wall -pthread")
ELSE()
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "i386")
        SET(CMAKE_CXX_FLAGS "-m32 -pipe -pthread -Wall ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-m32 -pipe -pthread -Wall ${CMAKE_C_FLAGS}")
    ELSE()
        SET(CMAKE_CXX_FLAGS "-m64 -pipe -pthread -Wall ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-m64 -pipe -pthread -Wall ${CMAKE_C_FLAGS}")
    ENDIF()
ENDIF()
MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "SET CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

# 检查c++0x c++11 c++14
IF(WIN32)
    SET(MSVC_2015 "_MSC_VER >= 1900")
    MESSAGE(STATUS "SET MSVC_2015 = ${MSVC_2015}")
    IF(MSVC_2015)
        ADD_DEFINITIONS(-D__ENABLE_CXX0X__
                        -D__ENABLE_CXX11__)
        MESSAGE(STATUS "compiler support c++11: -D__ENABLE_CXX11__ -D__ENABLE_CXX0X__")
    ENDIF()
ELSE()
    INCLUDE(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    IF(COMPILER_SUPPORTS_CXX14)
        MESSAGE(STATUS "COMPILER SUPPORT C++14")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
        ADD_DEFINITIONS(-D__ENABLE_CXX14__
                        -D__ENABLE_CXX11__
                        -D__ENABLE_CXX0X__)
    ELSEIF(COMPILER_SUPPORTS_CXX11)
        MESSAGE(STATUS "COMPILER SUPPORT C++11")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        ADD_DEFINITIONS(-D__ENABLE_CXX11__
                        -D__ENABLE_CXX0X__)
    ELSEIF(COMPILER_SUPPORTS_CXX0X)
        MESSAGE(STATUS "COMPILER SUPPORT C++0X")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        ADD_DEFINITIONS(-D__ENABLE_CXX0X__)
    ELSE()
        MESSAGE(STATUS "COMPILER SUPPORT C++98")
    ENDIF()
ENDIF()

IF(OS_LINUX)
    SET(CMAKE_SHARED_LINKER_FLAGS "-rdynamic -Wl,-E ${CMAKE_SHARED_LINKER_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
ENDIF()

# 检查构造类型
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    IF(WIN32)
        ADD_DEFINITIONS(-D_HAS_ITERATOR_DEBUGGING=0)
        MESSAGE(STATUS "-D_HAS_ITERATOR_DEBUGGING=0")
    ELSE()
        ADD_DEFINITIONS(-D_DEBUG)
        MESSAGE(STATUS "-D_DEBUG")
    ENDIF()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    IF(WIN32)
        ADD_DEFINITIONS(-D_RELEASE)
        MESSAGE(STATUS "-D_RELEASE")
    ELSE()
        ADD_DEFINITIONS(-D_RELEASE -DG_DISABLE_ASSERT)
        MESSAGE(STATUS "-D_RELEASE -DG_DISABLE_ASSERT")
        IF(CMAKE_COMPILER_IS_GNUCXX)
            SET(CMAKE_CXX_FLAGS "-fno-strict-aliasing ${CMAKE_CXX_FLAGS}")
            SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
            MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
            MESSAGE(STATUS "SET CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
        ENDIF()
        IF(OS_LINUX OR OS_DARWIN)
            SET(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
            SET(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
            SET(NEED_DEBUGINFO FALSE)
            MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
            MESSAGE(STATUS "SET CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
            MESSAGE(STATUS "SET NEED_DEBUGINFO = ${NEED_DEBUGINFO}")
        ENDIF()
    ENDIF()
ELSE()
    MESSAGE(FATAL_ERROR "MUST HAVE A BUILD TYPE")
ENDIF()

# 包含依赖库和连接文件
IF (DEFINED CUSTOM_DEPS_CMAKE_FILE)
    MESSAGE(STATUS "GET CUSTOM_DEPS_CMAKE_FILE = ${CUSTOM_DEPS_CMAKE_FILE}")
    INCLUDE(${CUSTOM_DEPS_CMAKE_FILE})
ENDIF()

MESSAGE(STATUS "\n**header.cmake end\n")
