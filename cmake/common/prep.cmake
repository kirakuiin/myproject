MESSAGE(STATUS "\n**include:prep.cmake\n")

####################################################################################################
MESSAGE(STATUS "\n**set var--------------------------------------------------------------------------")
# 设置临时变量
SET(MYCMAKE $ENV{MYCMAKE} CACHE STRING "ab cmake path")
SET(MYSRC $ENV{MYSRC} CACHE STRING "ab source path")
SET(MYDEPS $ENV{MYDEPS} CACHE STRING "ab depend path")
SET(MYTARGET $ENV{MYTARGET} CACHE STRING "ab target path")
SET(MKCPP $ENV{MKCPP} CACHE STRING "mk cpp path")
MESSAGE(STATUS "SET MYCMAKE = ${MYCMAKE}")
MESSAGE(STATUS "SET MYSRC = ${MYSRC}")
MESSAGE(STATUS "SET MYDEPS = ${MYDEPS}")
MESSAGE(STATUS "SET MYTARGET = ${MYTARGET}")
MESSAGE(STATUS "SET MKCPP = ${MKCPP}")

# 设置安装包生产的路径
IF(NOT DEFINED PACKAGE_PATH)
    SET(PACKAGE_PATH ${MYCMAKE}/packages CACHE type STRING)
    MESSAGE(STATUS "SET PACKAGE_PATH = ${PACKAGE_PATH}")
ENDIF()

# 设置 make package target 的默认存放路径
IF(NOT DEFINED INSTALL_PATH)
    SET(INSTALL_PATH .)
    MESSAGE(STATUS "SET INSTALL_PATH = ${INSTALL_PATH}")
ENDIF()

#设置if和else的控制开关选项
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
MESSAGE(STATUS "SET CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS = ${CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS}")

# 设置Cmake采用相对路径
SET(CMAKE_USE_RELATIVE_PATHS TRUE)
MESSAGE(STATUS "SET CMAKE_USE_RELATIVE_PATHS = ${CMAKE_USE_RELATIVE_PATHS}")

#将当前源文件目录自动添加到头文件引用路径下
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
MESSAGE(STATUS "SET CMAKE_INCLUDE_CURRENT_DIR = ${CMAKE_INCLUDE_CURRENT_DIR}")

#lhTodo
#设置“输出编译详细信息 = ON"
SET(CMAKE_VERBOSE_MAKEFILE ON)
MESSAGE(STATUS "SET CMAKE_VERBOSE_MAKEFILE = ${CMAKE_VERBOSE_MAKEFILE}")

# 设置cmake 的CMAKE_MODULE_PATH
SET(CMAKE_MODULE_PATH ${MYCMAKE}/modules)
MESSAGE(STATUS "SET CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

# 设置 CMAKE_PLATFORM_VERSION
SET(CMAKE_PLATFORM_VERSION $ENV{CMAKE_PLATFORM_VERSION} CACHE String "platform version")
MESSAGE(STATUS "SET CMAKE_PLATFORM_VERSION = ${CMAKE_PLATFORM_VERSION}")

#设置 CMAKE_BUILD_TYPE，
IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
    #在windows下makec这里CMAKE_BUILD_TYPE为空，此处设置
    SET(CMAKE_BUILD_TYPE "Debug")
    MESSAGE(STATUS "SET CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
ELSE()
    #在windows下makec release下直接在makec.bat中通过CMAKE_TYPE_FLAGS传给cmake的,此处就不为空
    MESSAGE(STATUS "GET CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
ENDIF()

# 检查编译的版本 x86和x64 (默认是空为64位)
MESSAGE(STATUS "GET CMAKE_BUILD_VERSION = ${CMAKE_BUILD_VERSION}")
IF("${CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "${CMAKE_BUILD_VERSION}" STREQUAL "x86" OR
    "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "x86")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "i386" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "power64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "power64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "power64" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "ppc64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "ppc64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "ppc64" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "pa64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "pa64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "pa64" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "sparcv9" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "sparcv9")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "sparcv9" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "sparc64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "sparc64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "sparc64" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "mips64el" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "mips64el")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "mips64el" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "ia64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "ia64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "ia64" CACHE STRING "build version" FORCE)
ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "aarch64" OR "$ENV{CMAKE_BUILD_VERSION}" STREQUAL "aarch64")
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "aarch64" CACHE STRING "build version" FORCE)
ELSE()
    UNSET(CMAKE_BUILD_VERSION)
    SET(CMAKE_BUILD_VERSION "x64" CACHE STRING "build version" FORCE)
ENDIF()
MESSAGE(STATUS "SET CMAKE_BUILD_VERSION = ${CMAKE_BUILD_VERSION}")


# 设置平台名称
SET(CMAKE_PLATFORM_NAME "$ENV{CMAKE_PLATFORM_NAME}" CACHE type STRING)
MESSAGE(STATUS "SET CMAKE_PLATFORM_NAME = ${CMAKE_PLATFORM_NAME}")

IF("${CMAKE_PLATFORM_NAME}" STREQUAL "Redhat")
    SET(REDHAT5 ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "NeoKylin")
    SET(NEOKYLIN ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "YHKylin")
    SET(YHKYLIN ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "AIX")
    SET(AIX ON)
    SET(OS_AIX ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "Solaris")
    SET(SOLARIS ON)
    SET(OS_SOLARIS ON)
ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "HP-UX")
    SET(HPUX ON)
    SET(HP-UX ON)
    SET(OS_HPUX ON)
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "pa64")
        SET(HPPA ON)
    ENDIF()
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "ia64")
        SET(HPIA ON)
    ENDIF()

    IF ("${CMAKE_PLATFORM_VERSION}" STREQUAL "11.23")
        SET(HPUX1123 ON)
    ENDIF()

    IF ("${CMAKE_PLATFORM_VERSION}" STREQUAL "11.31")
        SET(HPUX1131 ON)
    ENDIF()

ELSEIF("${CMAKE_PLATFORM_NAME}" STREQUAL "Linux")
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "K1")
        SET(K1 ON)
    ENDIF()
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "el6")
        SET(el6 ON)
    ENDIF()
    IF("${CMAKE_PLATFORM_VERSION}" STREQUAL "el7")
        SET(el7 ON)
    ENDIF()
ENDIF()

#设置OS类型
MESSAGE(STATUS "GET CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF(WIN32)
    SET(OS_WINDOWS ON)
    MESSAGE(STATUS "SET OS_WINDOWS = ${OS_WINDOWS}")
ELSEIF("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    SET(OS_LINUX ON)
    MESSAGE(STATUS "SET OS_LINUX = ${OS_LINUX}")
ENDIF()

#设置 MYPLATFORM
SET(MYPLATFORM ${CMAKE_PLATFORM_NAME}_${CMAKE_PLATFORM_VERSION}_${CMAKE_BUILD_VERSION} CACHE String "platform specific full name")
MESSAGE(STATUS "SET MYPLATFORM = ${MYPLATFORM}")

#设置 LIBS_PATH
IF(NOT DEFINED LIBS_PATH)
    SET(LIBS_PATH ${MYTARGET}/${CMAKE_BUILD_TYPE})
    MESSAGE(STATUS "SET LIBS_PATH = ${LIBS_PATH}")
ENDIF()



####################################################################################################
MESSAGE(STATUS "\n**define---------------------------------------------------------------------------")

IF(WIN32)
    # /Zc:wchar_t- 这个参数导致我们所有用到 wchar_t 的依赖库编译都需要加这个
    ADD_DEFINITIONS(/D _UNICODE
                    /D UNICODE
                    /D WIN32
                    /D __WINDOWS__
                    /D _CRT_SECURE_NO_WARNINGS
                    /D _CRT_SECURE_NO_DEPRECATE
                    # The following POSIX names for functions are deprecated.
                    /D _CRT_NONSTDC_NO_DEPRECATE
                )
ELSEIF(AIX)
    ADD_DEFINITIONS(-D__AIX__
                    -DAIX_VERS=502
                    -D_AIX)
ELSEIF(HPUX)
    IF(HPPA)
        IF(HPUX1131)
            ADD_DEFINITIONS(-D__HPPA_11_31__)
        ENDIF()
        ADD_DEFINITIONS(-D__HPPA__)
    ELSEIF(HPIA)
        ADD_DEFINITIONS(-D__HPIA__)
    ENDIF()
    ADD_DEFINITIONS(-D_XOPEN_SOURCE_EXTENDED
                    -D_GNU_SOURCE
                    -D_REENTRANT
                    -D__hpux
                    -DHPUX
                    -D__HPUX__)
ELSEIF(SOLARIS)
    ADD_DEFINITIONS(-D__SOLARIS__)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        ADD_DEFINITIONS(-D_GNU_SOURCE)
    ENDIF()
ELSEIF(NEOKYLIN)
    ADD_DEFINITIONS(-D__NEOKYLIN__
                    -D__LINUX__
                    -D_GNU_SOURCE)
    IF("$ENV{CMAKE_BUILD_VERSION}" STREQUAL "mips64el" OR "${CMAKE_BUILD_VERSION}" STREQUAL "mips64el")
        ADD_DEFINITIONS(-D__MIPS64__)
    ENDIF()
    IF("$ENV{CMAKE_BUILD_VERSION}" STREQUAL "sparcv9" OR "${CMAKE_BUILD_VERSION}" STREQUAL "sparcv9")
        ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1)
    ENDIF()
ELSEIF(YHKYLIN)
    ADD_DEFINITIONS(-D__YHKYLIN__ -D__LINUX__ -D_GNU_SOURCE)
ELSEIF(REDHAT5 OR el7)
        ADD_DEFINITIONS(-D__LINUX__ -D_GNU_SOURCE)
        IF("$ENV{CMAKE_BUILD_VERSION}" STREQUAL "i386" OR "${CMAKE_BUILD_VERSION}" STREQUAL "i386")
            ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1)
        ENDIF()
ELSEIF(K1)
        ADD_DEFINITIONS(-D__LINUX__ -D_GNU_SOURCE)
ELSEIF(el6)
        ADD_DEFINITIONS(-D__LINUX__ -D_GNU_SOURCE)
ENDIF()



IF(NOT NO_TESTENDIAN)
    IF(WIN32)
        ADD_DEFINITIONS(-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__)
        MESSAGE(STATUS "-D__LITTLE_ENDIAN__ -D__IS_LITTLE_ENDIAN__")
    ELSE()
        #C:\Program Files\CMake\share\cmake-3.7\Modules\TestBigEndian.cmake
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
ENDIF()





####################################################################################################
MESSAGE(STATUS "\n**funs_macros----------------------------------------------------------------------")
# 包含功能函数或者宏定义

##定义宏用来排除不需要编译的文件
MACRO(FILTER_OUT FILTERS INPUTS OUTPUTS)
   SET(FOUT "")
   FOREACH(INP ${INPUTS})
       SET(FILTERED 0)
       FOREACH(FILT ${FILTERS})
           IF(${FILTERED} EQUAL 0)
               IF("${FILT}" STREQUAL "${INP}")
                   SET(FILTERED 1)
               ENDIF("${FILT}" STREQUAL "${INP}")
           ENDIF(${FILTERED} EQUAL 0)
       ENDFOREACH(FILT ${FILTERS})
       IF(${FILTERED} EQUAL 0)
           SET(FOUT ${FOUT} ${INP})
       ENDIF(${FILTERED} EQUAL 0)
   ENDFOREACH(INP ${INPUTS})
   SET(${OUTPUTS} ${FOUT})
ENDMACRO(FILTER_OUT FILTERS INPUTS OUTPUTS)
MESSAGE(STATUS "MACRO(FILTER_OUT FILTERS INPUTS OUTPUTS)")



# 时间宏用来获取当前的时间 (以年月日方式显示)
MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C echo %DATE:~0,4%-%DATE:~5,2%-%DATE:~8,2%" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/(..).*" "\\1\\2\\3" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/(....).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSE (WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 00000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)
MESSAGE(STATUS "MACRO (TODAY RESULT)")



# 添加前缀宏用来给变量添加前缀
MACRO (ADD_PREFIX ORGN _prefix DEST)
    set(_dest "")
    foreach(_each_orgn ${ORGN})
        if("${_each_orgn}" STREQUAL "")
            message(WARNING "${_each_orgn} is null")
        else()
            set(_each_orgn ${_prefix}${_each_orgn})
            set(_dest ${_dest} ${_each_orgn})
        endif()
    set(_each_orgn "")
    endforeach(_each_orgn)
    set(${DEST} ${_dest})
ENDMACRO (ADD_PREFIX ORGN _prefix DEST)
MESSAGE(STATUS "MACRO (ADD_PREFIX ORGN _prefix DEST)")


#  macro name : translate_res_files
#  macro para : @sour = "the origin res dir" , usually xxx/res
#               @dest = "the destination res path" , usually xxx/locale
#               @CLEAN_FILES  "for make clean"
macro(translate_res_files sour dest CLEAN_FILES)
    IF(WIN32)
        SET(MSGFMT ${MYCMAKE}/tools/win/bin/msgfmt.exe)
    ELSE()
        SET(MSGFMT msgfmt)
    ENDIF()

    SET(LANG_DATA_PATH "locale")
    SET(LANG_TYPE "en_US" "zh_CN" "zh_TW_CN")
    SET(LANG_PATH_EXTEND "LC_MESSAGES")
    #SET(EXISTS_LANG_PATH_EXTEND FALSE)
    FOREACH(THE_LANG_TYPE ${LANG_TYPE})
        # python and c++
        SET(IN_LANG_REAL_PATH ${sour}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        SET(OUT_LANG_REAL_PATH ${dest}/${LANG_DATA_PATH}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        SET(INSTALL_LANG_PATH ${LANG_DATA_PATH}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        if(NOT EXISTS ${IN_LANG_REAL_PATH})
            SET(IN_LANG_REAL_PATH ${sour}/${THE_LANG_TYPE})
            SET(OUT_LANG_REAL_PATH ${dest}/${LANG_DATA_PATH}/${THE_LANG_TYPE})
            SET(INSTALL_LANG_PATH ${LANG_DATA_PATH}/${THE_LANG_TYPE})
        endif()
        # get po file list
        FILE(GLOB po_file_list "${IN_LANG_REAL_PATH}/*.po")
        FOREACH(the_po_file ${po_file_list})
            get_filename_component(the_po_file_name ${the_po_file} NAME_WE)
            if(EXISTS ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
                file(REMOVE_RECURSE ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
            endif()
            if(NOT EXISTS ${OUT_LANG_REAL_PATH})
                FILE(MAKE_DIRECTORY ${OUT_LANG_REAL_PATH})
            endif()
            MESSAGE(STATUS "${the_po_file} -o ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo")
            EXECUTE_PROCESS(COMMAND ${MSGFMT} ${the_po_file} -o ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo
                WORKING_DIRECTORY ${IN_LANG_REAL_PATH}
                RESULT_VARIABLE res_rvar ERROR_VARIABLE res_error
            )
            # ${res_rvar} =0 means success，= 1 means failed.
            IF(${res_rvar})
                MESSAGE("")
                MESSAGE("fatal_error:${res_error}")
                MESSAGE(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) ${the_po_file} failed")
            ENDIF()
            list(APPEND CLEAN_FILES ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
        ENDFOREACH()
    ENDFOREACH()
endmacro()
MESSAGE(STATUS "macro(translate_res_files sour dest CLEAN_FILES) ")




#  macro name : add_res_files_target
#  macro para :    @sour = "the origin res dir" , usually xxx/res
#                  @dest = "the destination res path" , usually xxx/locale
#                  @CLEAN_FILES  "for make clean"
macro(add_res_files_target sour dest CLEAN_FILES dest_name)
    IF(WIN32)
        SET(MSGFMT ${MYCMAKE}/tools/win/bin/msgfmt.exe)
    ELSE()
        SET(MSGFMT msgfmt)
    ENDIF()

    SET(LANG_DATA_PATH "locale")
    SET(LANG_TYPE "en_US" "zh_CN" "zh_TW_CN")
    SET(LANG_PATH_EXTEND "LC_MESSAGES")
    #SET(EXISTS_LANG_PATH_EXTEND FALSE)
    FOREACH(THE_LANG_TYPE ${LANG_TYPE})
        # c++
        SET(IN_LANG_REAL_PATH ${sour}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        SET(OUT_LANG_REAL_PATH ${dest}/${LANG_DATA_PATH}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        SET(INSTALL_LANG_PATH ${LANG_DATA_PATH}/${THE_LANG_TYPE}/${LANG_PATH_EXTEND})
        if(NOT EXISTS ${IN_LANG_REAL_PATH})
            SET(IN_LANG_REAL_PATH ${sour}/${THE_LANG_TYPE})
            SET(OUT_LANG_REAL_PATH ${dest}/${LANG_DATA_PATH}/${THE_LANG_TYPE})
            SET(INSTALL_LANG_PATH ${LANG_DATA_PATH}/${THE_LANG_TYPE})
        endif()
        # get po file list
        FILE(GLOB po_file_list "${IN_LANG_REAL_PATH}/*.po")
        FOREACH(the_po_file ${po_file_list})
            if("${dest_name}" STREQUAL "-")
                GET_FILENAME_COMPONENT(the_po_file_name ${the_po_file} NAME_WE)
            else()
                set(the_po_file_name ${dest_name})
            endif()
            if(EXISTS ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
                file(REMOVE_RECURSE ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
            endif()
            if(NOT EXISTS ${OUT_LANG_REAL_PATH})
                FILE(MAKE_DIRECTORY ${OUT_LANG_REAL_PATH})
            endif()
            ADD_CUSTOM_COMMAND(OUTPUT ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo
                               COMMAND ${MSGFMT} ${the_po_file} -o ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo
                               WORKING_DIRECTORY ${IN_LANG_REAL_PATH})
            LIST(APPEND CLEAN_FILES ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
            LIST(APPEND RES_FILES ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
            install_files(${INSTALL_LANG_PATH} ${OUT_LANG_REAL_PATH}/${the_po_file_name}.mo)
        ENDFOREACH()
    ENDFOREACH()
endmacro()
MESSAGE(STATUS "macro(add_res_files_target sour dest CLEAN_FILES dest_name) ")



#  macro name : add_file_target
#  macro para :      @targetpath = "the target path"
#                  @filepath = "the file path"
#                  @filename  "the file name"
macro(add_file_target targetpath filepath filename)
    if(NOT EXISTS ${LIBS_PATH}/${targetpath})
                FILE(MAKE_DIRECTORY ${LIBS_PATH}/${targetpath})
    endif()
    file(INSTALL ${filepath}/${filename} DESTINATION ${LIBS_PATH}/${targetpath}
                            FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                            GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                            )
    install_files(${targetpath} ${filepath}/${filename})
endmacro()
MESSAGE(STATUS "macro(add_file_target targetpath filepath filename) ")



#  function name : SOURCE_DIRS
#  function para : 支持可变参数列表
#    注:因为指定的目录可能是临时生成的，故取消对不存在目录的检查。
#       函数将所有相对路径处理成绝对路径方式显示
# example to use : SOURCE_DIRS(...)
function(SOURCE_DIRS)
    if(${ARGC} LESS 1)
        return()
    else()
        foreach(each_source_dir ${ARGN})
            # 由于IS_DIRECTORY对相对路径的处理不一定正确，故需要判断
            if("${each_source_dir}" STREQUAL ".")
                # 已添加当前路径
            elseif(IS_ABSOLUTE ${each_source_dir})
                #if(IS_DIRECTORY ${each_source_dir})
                    list(APPEND _source_dirs ${each_source_dir})
                #else()
                    #message(WARNING "${each_source_dir} does not exists or not a directory !!!")
                #endif()
            else() #相对路径
                #if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${each_source_dir})
                    list(APPEND _source_dirs ${CMAKE_CURRENT_SOURCE_DIR}/${each_source_dir})
                #else()
                    #message(WARNING "${each_source_dir} does not exists or not a directory !!!")
                #endif()
            endif()
        endforeach()
        set(SOURCE_DIRS ${SOURCE_DIRS} ${_source_dirs} PARENT_SCOPE)
    endif()
endfunction(SOURCE_DIRS)
MESSAGE(STATUS "function(SOURCE_DIRS)")




#  function name : thrift
#  function para :@module = "module name : cpp 、py、php ..."
#                  @thrift_file = "the name of the thrift file"
#                  @out_path = "the path of ouput (for default is gen-cpp(cpp)、gen-py(py) ...)"
# example to use : thrift (cpp xxx.thrift gen-cpp)
function(thrift module thrift_file out_path)
    set(THRIFT ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${out_path})
        file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/${out_path})
    endif()
    file(MAKE_DIRECTORY ${out_path})

    execute_process (COMMAND ${THRIFT} -v -r --gen ${module} --out ${out_path} ${thrift_file}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE ret_val
                    )
    # @ret_val  = 0 means success,others failed.
    if(${ret_val} EQUAL 0)
        message(STATUS "convert ${thrift_file} success.")
    else()
        message(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) Convert ${thrift_file} failed!!!")
    endif()

    # for `make clean`
    set(CLEAN_THRIFT_FILES  ${CLEAN_THRIFT_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${out_path} PARENT_SCOPE)
endfunction()
MESSAGE(STATUS "function(thrift module thrift_file out_path)")




#  function name : ADD_INSTALL_DIRS
#  function para : 支持可变参数列表
# example to use : ADD_INSTALL_DIRS(...)
function(ADD_INSTALL_DIRS)
    if(${ARGC} LESS 1)
        return()
    else()
        foreach(each_install_dir ${ARGN})
            # 由于IS_DIRECTORY对相对路径的处理不一定正确，故需要判断
            if(IS_ABSOLUTE ${each_install_dir})
                MESSAGE(FATAL_ERROR "Can not specify absolute paths !!!")
            else() #相对路径
                list(APPEND _install_dirs ${each_install_dir})
            endif()
        endforeach()
        # 增加多个安装路径
        set(INSTALL_SUB_PATH ${INSTALL_SUB_PATH} ${_install_dirs} PARENT_SCOPE)
    endif()
endfunction(ADD_INSTALL_DIRS)
MESSAGE(STATUS "function(ADD_INSTALL_DIRS)")



#  function name : install_file
#  function para :@source = "source files" , where the dirs from, if the dir is ended with
#                             '/', that means to copy @source/*
#                  @dest = "destination" , where the dirs go to
function(install_file source dest)
    if(${ARGC} LESS 1)
        message(FATAL_ERROR "install_file do not support arguments less than 2!")
    #elseif(${ARGC} = 2)
    #    set(_file_permissions OWNER_EXECUTE OWNER_WRITE OWNER_READ
    #                         GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
    #else()
    #    set(_file_permissions ${ARGN})
    endif()

    if(DEFINED dest AND IS_ABSOLUTE ${dest})
            message(FATAL_ERROR "install_file do not support absolute path!")
    endif()

    FOREACH(subdir ${INSTALL_SUB_PATH})
        if(NOT "${subdir}" STREQUAL ".")
            set(target ${INSTALL_PATH}/${subdir})
        else()
            set(target ${INSTALL_PATH})
        endif()

        if(NOT "${dest}" STREQUAL "." AND NOT "${dest}" STREQUAL "")
            set(target ${target}/${dest})
        endif()

        INSTALL(FILES ${source} DESTINATION ${target}
                PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                            GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                )
    ENDFOREACH(subdir)
endfunction()
MESSAGE(STATUS "function(install_file source dest)")






function(install_files dest source)
list(LENGTH ARGV  argv_len)
#message(STATUS "length of ARGV: ${argv_len} +++++++[${dest}]++++++")
set(i 1)
while( i LESS ${argv_len})
     list(GET ARGV ${i} argv_value)
     #message(STATUS "argv${i}: ${argv_value}")
     install_file(${argv_value} ${dest})
     math(EXPR i "${i} + 1")
endwhile()
endfunction()
MESSAGE(STATUS "function(install_files dest source)")




function(install_dirs dest source)
list(LENGTH ARGV  argv_len)
#message(STATUS "length of ARGV: ${argv_len} ${dest}++++++++++++")
set(i 1)
while( i LESS ${argv_len})
     list(GET ARGV ${i} argv_value)
     #message(STATUS "argv${i}: ${argv_value}")
     install_dir(${argv_value} ${dest})
     math(EXPR i "${i} + 1")
endwhile()
endfunction()
MESSAGE(STATUS "function(install_dirs dest source)")




#  function name : install_dir
#  function para :@source = "source dir" , where the dirs from, if the dir is ended with
#                             '/', that means to copy @source/*
#                  @dest = "destination" , where the dirs go to
function(install_dir source dest)
    if(${ARGC} LESS 1)
        message(FATAL_ERROR "install_dir do not support arguments less than 2!")
    #elseif(${ARGC} = 2)
    #    set(_dir_permissions PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    #                         GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
    #else()
    #    set(_dir_permissions ${ARGN})
    endif()

    if(DEFINED dest AND IS_ABSOLUTE ${dest})
            message(STATUS "-------------------${dest}-----------------")
            message(FATAL_ERROR "install_dir do not support absolute path!")
    endif()

    FOREACH(subdir ${INSTALL_SUB_PATH})
        if(NOT "${subdir}" STREQUAL ".")
            set(target ${INSTALL_PATH}/${subdir})
        else()
            set(target ${INSTALL_PATH})
        endif()

        if(NOT "${dest}" STREQUAL "." AND NOT "${dest}" STREQUAL "")
            set(target ${target}/${dest})
        endif()

        #message(STATUS "++++++++++++++++++dest: ${dest}+${target}+++++++++++++++++++++")
        IF(AIX)
        install(DIRECTORY ${source} DESTINATION ${target}
                    PATTERN "*.pdb" EXCLUDE
                    PATTERN "*.lib" EXCLUDE
                    PATTERN "*.def" EXCLUDE
                    PATTERN "*.po" EXCLUDE
                    PATTERN "*.sql" EXCLUDE
                    PATTERN "*.dll.manifest" EXCLUDE
                    PATTERN "CMakeLists.txt" EXCLUDE
                    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                                GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                )
        ELSE()
        install(DIRECTORY ${source} DESTINATION ${target}
                    PATTERN "*.lib" EXCLUDE
                    PATTERN "*.def" EXCLUDE
                    PATTERN "*.a" EXCLUDE
                    PATTERN "*.po" EXCLUDE
                    PATTERN "*.sql" EXCLUDE
                    PATTERN "*.dll.manifest" EXCLUDE
                    PATTERN "CMakeLists.txt" EXCLUDE
                    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                                GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                )
        ENDIF()
    ENDFOREACH(subdir)
endfunction()
MESSAGE(STATUS "function(install_dir source dest)")




#  function name : install_target
#  function para :@source = "source dir" , where the dirs from, if the dir is ended with
#                             '/', that means to copy @source/*
#                  @dest = "destination" , where the dirs go to
function(install_target type target_name dest)
    if(${ARGC} LESS 2)
        message(FATAL_ERROR "install_target do not support arguments less than 3!")
    endif()

    if(DEFINED dest AND IS_ABSOLUTE ${dest})
        message(FATAL_ERROR "install_target do not support absolute path!")
    endif()

    FOREACH(subdir ${INSTALL_SUB_PATH})
        if(NOT "${subdir}" STREQUAL ".")
            set(target ${INSTALL_PATH}/${subdir})
        else()
            set(target ${INSTALL_PATH})
        endif()

        if(NOT "${dest}" STREQUAL "." AND NOT "${dest}" STREQUAL "")
            set(target ${target}/${dest})
        endif()

        INSTALL(TARGETS ${target_name} ${type} DESTINATION ${target}
                PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                            GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                )
        IF(WIN32)
            INSTALL(FILES ${LIBS_PATH}/${dest}/${target_name}.pdb DESTINATION ${target}
                            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                                GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
            )
        ENDIF()
    ENDFOREACH(subdir)
endfunction()
MESSAGE(STATUS "function(install_target type target_name dest)")




#  function name : seek_deps_library
#  function para :@path1 = "the output path dir" , first searching path
#                  @path2 = "the original path" , second searching path
#                    @ ...  library to seek,must be 1 or more
function(seek_deps_library_no_install path1 path2)
    if(${ARGC} LESS 3)
        message(FATAL_ERROR "Parameter error,need three or more parameters.")
    else()
        # 第一次从${path1}查找
        find_library(FIRST_LIB NAMES ${ARGN} PATHS ${path1} NO_DEFAULT_PATH)
            if("${FIRST_LIB}" STREQUAL "FIRST_LIB-NOTFOUND")
                # 第二次从${path2}查找
                find_library(SECOND_LIB NAMES ${ARGN} PATHS ${path2} NO_DEFAULT_PATH)
                if("${SECOND_LIB}" STREQUAL "SECOND_LIB-NOTFOUND")
                    message(FATAL_ERROR "Can not found lib ${ARGN}")
                else()
                #message(STATUS "found lib ${ARGN} in ${path2}")
                file(INSTALL ${path2}/ DESTINATION ${path1}
                            FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                                GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                        )
                endif()
                # 清除缓存以便下次重新查找
                unset(SECOND_LIB CACHE)
            else()
                #message(STATUS "found lib ${ARGN} in ${path1} values [${FIRST_LIB}]")
            endif()
        # 清除缓存以便下次重新查找
        unset(FIRST_LIB CACHE)
    endif()
endfunction()
MESSAGE(STATUS "function(seek_deps_library_no_install path1 path2)")



#  function name : seek_base_library
#  function para : @path1 = "the output path dir", first searching path
#                  @ ...  library to seek, must be 1 or more
function(seek_base_library_no_install path1)
    if(${ARGC} LESS 2)
        message(FATAL_ERROR "Parameter error,need three or more parameters.")
    else()
        SET(LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}")
        SET(LIBRARY_SUFFIXES "${CMAKE_FIND_LIBRARY_SUFFIXES}")
        find_library(FIRST_LIB NAMES ${ARGN} PATHS ${path1} NO_DEFAULT_PATH)
        if("${FIRST_LIB}" STREQUAL "FIRST_LIB-NOTFOUND")
            message(STATUS "Can not found lib ${ARGN} in ${path2}")
        else()
            file(INSTALL ${SECOND_LIB} DESTINATION ${path1}
                        USE_SOURCE_PERMISSIONS
                )
        endif()
        SET(CMAKE_FIND_LIBRARY_PREFIXES "${LIBRARY_PREFIXES}")
        SET(CMAKE_FIND_LIBRARY_SUFFIXES "${LIBRARY_SUFFIXES}")
    endif()
    # 清除缓存以便下次重新查找
    unset(FIRST_LIB CACHE)
endfunction()
MESSAGE(STATUS "function(seek_base_library_no_install path1)")




#  function name : seek_deps_library
#  function para :@path1 = "the output path dir" , first searching path
#                  @path2 = "the original path" , second searching path
#                    @ ...  library to seek,must be 1 or more
function(seek_deps_library path1 path2)
    seek_deps_library_no_install (${path1} ${path2} ${ARGN})
    if(ENABLE_PACKAGE)
        install_dir (${path2}/ .)
    endif()
endfunction()
MESSAGE(STATUS "function(seek_deps_library path1 path2)")




## 拷贝文件到指定目录，但是不打入安装包
#  function name : seek_deps_file_no_install
#  function para :@path1 = "the output path dir" , first searching path
#                  @path2 = "the original path" , second searching path
#                    @ ...  file to seek,must be 1 or more
function(seek_deps_file_no_install path1 path2)
    if(${ARGC} LESS 3)
        message(FATAL_ERROR "Parameter error, seek_deps_file_no_install need three or more parameters.")
    else()
        # 第一次从${path1}查找
        find_file(FIRST_FILE NAMES ${ARGN} PATHS ${path1} NO_DEFAULT_PATH)
            if("${FIRST_FILE}" STREQUAL "FIRST_FILE-NOTFOUND")
                # 第二次从${path2}查找
                find_file(SECOND_FILE NAMES ${ARGN} PATHS ${path2} NO_DEFAULT_PATH)
                    if("${SECOND_FILE}" STREQUAL "SECOND_FILE-NOTFOUND")
                        message(FATAL_ERROR "Can not found file ${ARGN}")
                    else()
                        file(INSTALL ${path2}/ DESTINATION ${path1}
                                FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                                    GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
                            )
                    endif()
                # 清除缓存以便下次重新查找
                unset(SECOND_FILE CACHE)
            endif()
        # 清除缓存以便下次重新查找
        unset(FIRST_FILE CACHE)
    endif()
endfunction()
MESSAGE(STATUS "function(seek_deps_file_no_install path1 path2)")





#  function name : seek_deps_file
#  function para :@path1 = "the output path dir" , first searching path
#                  @path2 = "the original path" , second searching path
#                    @ ...  file to seek,must be 1 or more
function(seek_deps_file path1 path2)
    seek_deps_file_no_install(${path1} ${path2} ${ARGN})
    if(ENABLE_PACKAGE)
        install_dir (${path2}/ .)
    endif()
endfunction()
MESSAGE(STATUS "function(seek_deps_file path1 path2)")



#  function name : thrift
#  function para :@module = "module name : cpp/py/php ..."
#                  @thrift_file_path = "a path to thrift file"
#                  @out_path = "the path of ouput (for default is gen-cpp(cpp)、gen-py(py) ...)"
# example to use : thrift (cpp xxx.thrift gen-cpp)
function(convert_thrift_by_path module thrift_file_path out_path)
    set(THRIFT ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)

    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${out_path})
        file(MAKE_DIRECTORY ${out_path})
    endif()

    #deps thrift paths
    IF(DEFINED THRIFT_DEPS_PATHS)
        FOREACH(each_deps_path ${THRIFT_DEPS_PATHS})
            list(APPEND deps_thrift_flags -I ${each_deps_path})
        ENDFOREACH()
    ENDIF()

    FILE(GLOB thrift_file_list "${thrift_file_path}/*.thrift")

    message(status "CMD: ${THRIFT} -v -r ${deps_thrift_flags} --gen ${module} --out ${out_path} ${thrift_file}")

    FOREACH(thrift_file ${thrift_file_list})
        execute_process (COMMAND ${THRIFT} -v -r ${deps_thrift_flags} --gen ${module} --out ${out_path} ${thrift_file}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE ret_val
                        )

        # @ret_val  = 0 means success,others failed.
        if(${ret_val} EQUAL 0)
            message(STATUS "convert ${thrift_file} success.")
        else()
            message(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) Convert ${thrift_file} failed!!!")
        endif()
    ENDFOREACH()

    # for `make clean`
    SET(ADDITIONAL_CLEAN_FILES ${ADDITIONAL_CLEAN_FILES};${CMAKE_CURRENT_SOURCE_DIR}/${out_path} PARENT_SCOPE)
endfunction()
MESSAGE(STATUS "function(convert_thrift_by_path module thrift_file_path out_path)")




#  function name : thrift
#  function para :@module = "module name : cpp/py/php ..."
#                  @thrift_file_path = "a path to thrift file"
#                  @out_path = "the path of ouput (for default is gen-cpp(cpp)、gen-py(py) ...)"
# example to use : thrift (cpp xxx.thrift gen-cpp)
function(convert_thrift_by_file module thrift_file_name out_path)
    set(THRIFT ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)

    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${out_path})
        file(MAKE_DIRECTORY ${out_path})
    endif()

    #deps thrift paths
    IF(DEFINED THRIFT_DEPS_PATHS)
        FOREACH(each_deps_path ${THRIFT_DEPS_PATHS})
            list(APPEND deps_thrift_flags -I ${each_deps_path})
        ENDFOREACH()
    ENDIF()

    FILE(GLOB thrift_file_list "${thrift_file_name}")

    FOREACH(thrift_file ${thrift_file_list})
        execute_process (COMMAND ${THRIFT} -v -r ${deps_thrift_flags} --gen ${module} --out ${out_path} ${thrift_file}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE ret_val
                        )

        # @ret_val  = 0 means success,others failed.
        if(${ret_val} EQUAL 0)
            message(STATUS "convert ${thrift_file} success.")
        else()
            message(FATAL_ERROR "(LINE:${CMAKE_CURRENT_LIST_LINE}) Convert ${thrift_file} failed!!!")
        endif()
    ENDFOREACH()

    # for `make clean`
    SET(ADDITIONAL_CLEAN_FILES ${ADDITIONAL_CLEAN_FILES};${CMAKE_CURRENT_SOURCE_DIR}/${out_path} PARENT_SCOPE)
endfunction()
MESSAGE(STATUS "function(convert_thrift_by_file module thrift_file_name out_path)")






####################################################################################################
MESSAGE(STATUS "\n**compile_thrift-------------------------------------------------------------------")

FUNCTION(COMPILE_THRIFT THRIFT_FILES THRIFT_OUT_DIR DEPS_THRIFT_PATH SRCS HDRS)
    FILE(MAKE_DIRECTORY ${THRIFT_OUT_DIR})
    SET(thrift_dir ${THRIFT_OUT_DIR}/gen-cpp)

    IF(WIN32)
        SET(THRIFTCMD ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift.exe)
    ELSE()
        SET(THRIFTCMD ${MYDEPS}/thrift/bin/${MYPLATFORM}/thrift)
        EXECUTE_PROCESS(COMMAND chmod +x ${THRIFTCMD})
    ENDIF()

    MESSAGE(STATUS "compiling thrift files....")

    SET(${SRCS})
    SET(${HDRS})

    FOREACH(tft_file ${THRIFT_FILES})
        get_filename_component(ABS_FIL ${tft_file} ABSOLUTE)
        get_filename_component(FIL_WE ${tft_file} NAME_WE)
        list(APPEND ${HDRS} "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.h")
        list(APPEND ${SRCS} "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.cpp")

        LIST(APPEND ${CLEAN_THRIFT_FILES} "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.h")
        LIST(APPEND ${CLEAN_THRIFT_FILES} "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.cpp")

        # 所有 thrift 都依赖 basedatatype，添加到默认查找路径中
        ADD_CUSTOM_COMMAND(OUTPUT "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.h"
                                  "${THRIFT_OUT_DIR}/gen-cpp/${FIL_WE}.cpp"
                           COMMAND ${THRIFTCMD}
                           ARGS -o ${THRIFT_OUT_DIR} -I ${DEPS_THRIFT_PATH} -I $ENV{BASETHRIFTDTS} --gen cpp ${tft_file}
                           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                           COMMENT ""
                           )
    ENDFOREACH()

    MESSAGE(STATUS "Compiling thrift files SUCCESS ...")

    INCLUDE_DIRECTORIES(${THRIFT_OUT_DIR})
    INCLUDE_DIRECTORIES(${thrift_dir})
    SET_SOURCE_FILES_PROPERTIES(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    SET(${SRCS} ${${SRCS}} PARENT_SCOPE)
    SET(${HDRS} ${${HDRS}} PARENT_SCOPE)

ENDFUNCTION()
MESSAGE(STATUS "FUNCTION(COMPILE_THRIFT THRIFT_FILES THRIFT_OUT_DIR DEPS_THRIFT_PATH SRCS HDRS)")




## 编译THRIFT文件
MACRO(ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR GEN_DIR_NAME DEPS_THRIFT_PATH)
    #MESSAGE(STATUS "${ARGN}.............")
    FOREACH(thrift_file ${ARGN})
        IF(NOT DEFINED DEPS_THRIFT_PATH)
            SET(DEPS_THRIFT_PATH ".")
        ENDIF()
        COMPILE_THRIFT(${thrift_file} ${PROJECT_BINARY_DIR}/${GEN_DIR_NAME} ${DEPS_THRIFT_PATH} CUR_THRIFT_GEN_CPP_SRCS CUR_THRIFT_GEN_CPP_HDRS)
        SET(THRIFT_GEN_CPP_SRCS ${THRIFT_GEN_CPP_SRCS}
                                ${CUR_THRIFT_GEN_CPP_SRCS})
        SET(THRIFT_GEN_CPP_HDRS ${THRIFT_GEN_CPP_HDRS}
                                ${CUR_THRIFT_GEN_CPP_HDRS})
    ENDFOREACH()
    MESSAGE(STATUS "Adding thrift files dir ${PROJECT_BINARY_DIR}/${GEN_DIR_NAME}/gen-cpp")
ENDMACRO()
MESSAGE(STATUS "MACRO(ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR GEN_DIR_NAME DEPS_THRIFT_PATH)")


MACRO(ADD_BUILD_THRIFT_FILES)
    ADD_BUILD_THRIFT_FILES_WITH_GEN_DIR(project_thrift . ${ARGN})
ENDMACRO()
MESSAGE(STATUS "MACRO(ADD_BUILD_THRIFT_FILES)")




####################################################################################################
MESSAGE(STATUS "\n**set cmake------------------------------------------------------------------------")

# 设置 cmake rpath 选项
SET(CMAKE_INSTALL_RPATH .;..)
MESSAGE(STATUS "SET CMAKE_INSTALL_RPATH = ${CMAKE_INSTALL_RPATH}")


SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
MESSAGE(STATUS "SET CMAKE_BUILD_WITH_INSTALL_RPATH = ${CMAKE_BUILD_WITH_INSTALL_RPATH}")

# 设置cmake install属性
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
MESSAGE(STATUS "SET CMAKE_INSTALL_RPATH_USE_LINK_PATH = ${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")

# 设置默认的打包子路径
SET(INSTALL_SUB_PATH ".")
MESSAGE(STATUS "SET INSTALL_SUB_PATH = ${INSTALL_SUB_PATH}")

# 默认源文件目录，当前目录
SET(SOURCE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "the source files dirs")
MESSAGE(STATUS "SET SOURCE_DIRS = ${SOURCE_DIRS}")

#
# 设置选项来打开或关闭功能
#
# 设置`make package`选项，默认为 ON.
IF(NOT DEFINED ENABLE_PACKAGE)
    MESSAGE(STATUS "GET ENABLE_PACKAGE = ${ENABLE_PACKAGE}")
    OPTION(ENABLE_PACKAGE "enable make package to install libs" ON)
ENDIF()
MESSAGE(STATUS "GET ENABLE_PACKAGE = ${ENABLE_PACKAGE}")

# 代码覆盖率选项，默认为OFF
IF(NOT DEFINED ENABLE_GCOV)
    OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)
ENDIF()
MESSAGE(STATUS "GET ENABLE_GCOV = ${ENABLE_GCOV}")


# 性能剖析选项，默认为OFF
IF(NOT DEFINED ENABLE_PROFILE)
    OPTION(ENABLE_PROFILE "Enable profile (profile option)" OFF)
ENDIF()
MESSAGE(STATUS "GET ENABLE_PROFILE = ${ENABLE_PROFILE}")


# 设置可执行文件是否自动执行选项，默认为 OFF.
IF(NOT DEFINED AUTO_RUN)
    OPTION(AUTO_RUN "auto running executable file" OFF)
ENDIF()
MESSAGE(STATUS "GET AUTO_RUN = ${AUTO_RUN}")


# 设置支持交叉编译，默认为 OFF.
IF(NOT DEFINED XCOMPILE)
    OPTION(XCOMPILE "Using cross platform compilation" OFF)
ELSE()
    # @cx, 跨平台编译需要设置不同的查找路径
    SET(ENV{LD_LIBRARY_PATH} /usr/local/lib:$ENV{LD_LIBRARY_PATH})
    MESSAGE(STATUS "SET LD_LIBRARY_PATH = ENV{LD_LIBRARY_PATH}")
ENDIF()
MESSAGE(STATUS "GET XCOMPILE = ${XCOMPILE}")


#PACKAGE_PATH for `make package`
IF(NOT DEFINED PACKAGE_PATH)
    SET(PACKAGE_PATH ${MYCMAKE}/packages CACHE STRING "package path")
    MESSAGE(STATUS "SET PACKAGE_PATH = ${PACKAGE_PATH}")
ENDIF()
MESSAGE(STATUS "GET PACKAGE_PATH = ${PACKAGE_PATH}")





####################################################################################################
MESSAGE(STATUS "\n**check----------------------------------------------------------------------------")

IF(WIN32)
    # http://msdn.microsoft.com/en-us/library/bkb78zf3(v=vs.90).aspx 多重继承成员指针问题
    # 以下警告只显示一次
    # https://msdn.microsoft.com/en-us/library/fsk896zz.aspx Debug a Release Build
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /vmm /vmg /MP /wo4067 /wo4244 /wo4267 /wo4181 /wd4099")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /INCREMENTAL:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

    MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS_RELEASE = ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
    MESSAGE(STATUS "SET CMAKE_EXE_LINKER_FLAGS_RELEASE = ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

    STRING(REPLACE "/EHsc" "/EHa /GS" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

ELSEIF(AIX)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET(CMAKE_C_FLAGS "-maix64 -mpowerpc64 -pipe -pthread ${CMAKE_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "-maix64 -mpowerpc64 -pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_SHARED_LIBRARY_RUNTIME_CXX_FLAG "-Wl,-bbigtoc -Wl,-blibpath:/usr/lib/threads:")
        SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -X64 cr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -X64 cr <TARGET> <LINK_FLAGS> <OBJECTS>")
    ENDIF()
ELSEIF(NEOKYLIN)
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "sparcv9")
        SET(CMAKE_CXX_FLAGS "-m32 -mcpu=niagara2 -pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-m32 -mcpu=niagara2 -pipe -pthread ${CMAKE_C_FLAGS}")
    ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "mips64el")
        SET(CMAKE_CXX_FLAGS "-pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-pipe -pthread ${CMAKE_C_FLAGS}")
    ENDIF()
ELSEIF(HPUX)
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "pa64" AND CMAKE_COMPILER_IS_GNUCXX)
        SET(CMAKE_C_FLAGS "-mpa-risc-2-0 -march=2.0 -pipe -pthread ${CMAKE_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "-mpa-risc-2-0 -march=2.0 -pipe -pthread ${CMAKE_CXX_FLAGS}")
    ENDIF()
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "ia64" AND CMAKE_COMPILER_IS_GNUCXX)
        SET(CMAKE_C_FLAGS "-mlp64 -pipe -pthread ${CMAKE_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "-mlp64 -pipe -pthread ${CMAKE_CXX_FLAGS}")
    ENDIF()
ELSEIF(SOLARIS)
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "sparc64" AND CMAKE_COMPILER_IS_GNUCXX)
        SET(CMAKE_CXX_FLAGS "-m64 -pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-m64 -pipe -pthread ${CMAKE_C_FLAGS}")
         SET(CMAKE_SHARED_LINKER_FLAGS "-m64 -pipe -pthread ${CMAKE_SHARED_LINKER_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS "-m64 -pipe -pthread ${CMAKE_EXE_LINKER_FLAGS}")
        SET(CMAKE_AR "/usr/ccs/bin/ar")
        # cmake linker bug
        SET(CMAKE_CXX_CREATE_SHARED_LIBRARY "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS>  <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
    ENDIF()
ELSE()
    # 检查编译的版本 x86和x64 (默认是空为64位)
    IF("${CMAKE_BUILD_VERSION}" STREQUAL "i386")
        SET(CMAKE_CXX_FLAGS "-m32 -pipe -pthread -Wall ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-m32 -pipe -pthread -Wall ${CMAKE_C_FLAGS}")
    ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "ia64")
        SET(CMAKE_CXX_FLAGS "-pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-pipe -pthread ${CMAKE_C_FLAGS}")
    ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "ppc64")
        SET(CMAKE_CXX_FLAGS "-mpowerpc64 -pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-mpowerpc64 -pipe -pthread ${CMAKE_C_FLAGS}")
    ELSEIF("${CMAKE_BUILD_VERSION}" STREQUAL "aarch64")
        SET(CMAKE_CXX_FLAGS "-pipe -pthread ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "-pipe -pthread ${CMAKE_C_FLAGS}")
    ELSE()
        SET(CMAKE_C_FLAGS "-m64 -pipe -pthread -Wall ${CMAKE_CXX_FLAGS}")
        SET(CMAKE_CXX_FLAGS "-m64 -pipe -pthread -Wall ${CMAKE_CXX_FLAGS}")
    ENDIF()
    MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

ENDIF()

# 检查c++0x c++11 c++14标准
IF(WIN32)
    SET(MSVC_2015 "_MSC_VER >= 1900")
    MESSAGE(STATUS "SET MSVC_2015 = ${MSVC_2015}")

    # 不太懂好吧
    IF(MSVC_2015)
        ADD_DEFINITIONS(-D__ENABLE_CXX11__
                        -D__ENABLE_CXX0X__)
        MESSAGE(STATUS "compiler support c++11: -D__ENABLE_CXX11__ -D__ENABLE_CXX0X__")
    ENDIF()
ELSE()
    #C:\Program Files\CMake\share\cmake-3.7\Modules\CheckCXXCompilerFlag.cmake
    INCLUDE(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    IF(COMPILER_SUPPORTS_CXX14)
        MESSAGE(STATUS "compiler support c++14 ......")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
        ADD_DEFINITIONS(-D__ENABLE_CXX14__
                        -D__ENABLE_CXX11__
                        -D__ENABLE_CXX0X__)
    ELSEIF(COMPILER_SUPPORTS_CXX11)
        MESSAGE(STATUS "compiler support c++11 ......")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        ADD_DEFINITIONS(-D__ENABLE_CXX11__
                        -D__ENABLE_CXX0X__)
    ELSEIF(COMPILER_SUPPORTS_CXX0X)
        MESSAGE(STATUS "compiler support c++0x ......")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        ADD_DEFINITIONS(-D__ENABLE_CXX0X__)
    ELSE()
        MESSAGE(STATUS "compiler support c++98 ......")
    ENDIF()
ENDIF()


IF(OS_LINUX)
    SET(CMAKE_SHARED_LINKER_FLAGS "-rdynamic -Wl,-E ${CMAKE_SHARED_LINKER_FLAGS}")
    MESSAGE(STATUS "SET CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
ENDIF()

#check build type
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    IF(WIN32)
        #默认为 /MDd 编译选项
        ADD_DEFINITIONS(-D_HAS_ITERATOR_DEBUGGING=0)
        MESSAGE(STATUS "-D_HAS_ITERATOR_DEBUGGING=0")
    ELSE()
        ADD_DEFINITIONS(-D_DEBUG)
        MESSAGE(STATUS "-D_DEBUG")
    ENDIF()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    IF(WIN32)
        # #默认为 /MD 编译选项
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
        IF(OS_LINUX)
            SET(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
            SET(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
            SET(NEED_DEBUGINFO TRUE)
            MESSAGE(STATUS "SET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
            MESSAGE(STATUS "SET CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
            MESSAGE(STATUS "SET NEED_DEBUGINFO = ${NEED_DEBUGINFO}")
        ENDIF()
    ENDIF()
ELSE()
    MESSAGE(FATAL_ERROR "Must have a build type")
ENDIF()


#包含依赖库查找/链接文件
IF (DEFINED CUSTOM_DEPS_CMAKE_FILE)
    #包含用户自定义的依赖库
    MESSAGE(STATUS "GET CUSTOM_DEPS_CMAKE_FILE = ${CUSTOM_DEPS_CMAKE_FILE}")
    INCLUDE(${CUSTOM_DEPS_CMAKE_FILE})
ENDIF()


####################################################################################################
MESSAGE(STATUS "\n**prep.cmake end----------------------------------------------------------------")
MESSAGE(STATUS "")
MESSAGE(STATUS "")
