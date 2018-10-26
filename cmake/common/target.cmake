MESSAGE(STATUS "\n\n**include:target.cmake\n")

####################################################################################################
MESSAGE(STATUS "\n**deps-----------------------------------------------------------------------------")


# 检查是否存在config目录,并将目录文件到生成lib库所在的目录下
# 仅仅有拷贝的动作，故配置文件可以放置在config目录
SET(DEFAULT_CONFIG_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/config)
MESSAGE(STATUS "SET DEFAULT_CONFIG_PATH = ${DEFAULT_CONFIG_PATH}")
IF(EXISTS ${DEFAULT_CONFIG_PATH})
    FILE(INSTALL ${DEFAULT_CONFIG_PATH}/ DESTINATION ${LIBS_PATH} PATTERN ".svn" EXCLUDE)
    install_dir(${DEFAULT_CONFIG_PATH}/ "")
    MESSAGE(STATUS "install_dir(${DEFAULT_CONFIG_PATH})")
ENDIF()


# 检查是否存在service目录,并将目录文件拷贝到生成lib库所在的目录下
# 仅仅有拷贝的动作，故服务文件可以放置在各模块所在的service目录
SET(DEFAULT_SERVICE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/service)
MESSAGE(STATUS "SET DEFAULT_SERVICE_PATH = ${DEFAULT_SERVICE_PATH}")
IF(EXISTS ${DEFAULT_SERVICE_PATH})
    IF(EXISTS ${DEFAULT_SERVICE_PATH}/${ABPLATFORM})
        FILE(INSTALL ${DEFAULT_SERVICE_PATH}/${ABPLATFORM}/ DESTINATION ${LIBS_PATH})
        install_dir(${DEFAULT_SERVICE_PATH}/${ABPLATFORM}/ "")
    ENDIF()
    MESSAGE(STATUS "install_dir(DEFAULT_SERVICE_PATH)")
ENDIF()


# 检查目录下是否有 images 目录
SET(DEFAULT_IMAGES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/images)
MESSAGE(STATUS "SET DEFAULT_IMAGES_PATH = ${DEFAULT_IMAGES_PATH}")
IF(EXISTS ${DEFAULT_IMAGES_PATH})
    FILE(INSTALL ${DEFAULT_IMAGES_PATH} DESTINATION ${LIBS_PATH})
    install_dir(${DEFAULT_IMAGES_PATH} "")
    MESSAGE(STATUS "install_dir(${DEFAULT_IMAGES_PATH})")
ENDIF()


# 设置所有依赖库选项
# 注:可以在相应的NEED_XXX下，设置其他依赖，这样就不用
# 多次在CMakeLists.txt中设置其他NEED_XXX选项。


# 配置 boost 相关依赖
IF(${NEED_BOOST})
    FIND_PACKAGE(boost REQUIRED)
ENDIF()

# 配置 googletest 相关依赖
IF(${NEED_GTEST})
    FIND_PACKAGE(gtest REQUIRED)
ENDIF()


#
# 配置 内部相关依赖
#
#默认包含引用目录
INCLUDE_DIRECTORIES(${MKCPP};)

# 依赖 NEED_BASECOMMON
IF(${NEED_BASECOMMON})
    MESSAGE(STATUS "basecommon is used")
    ADD_DEFINITIONS(-D__USING_BASECORE__)
    seek_base_library_no_install(${LIBS_PATH} basecommon)
    INCLUDE_DIRECTORIES(${MKCPP}/basecommon/include;
                        ${MKCPP}/basecommon/src;)
    SET(LINK_BASECORE_LIBS basecommon)
ENDIF()

# 依赖 APPCORE
IF(${NEED_APPCORE})
    FIND_PACKAGE(basecore REQUIRED)
    FIND_PACKAGE(appcore REQUIRED)
    SET(CUSTOM_IDL_PATH ${CUSTOM_IDL_PATH};$ENV{APPCORE}/public)
    INCLUDE_DIRECTORIES($ENV{APPCORE};$ENV{APPCORE}/public;$ENV{APPCORE}/include)
ENDIF()

# 依赖 DATACAHNNEL
IF (${NEED_DATACHANNEL})
    SET(CUSTOM_IDL_PATH ${CUSTOM_IDL_PATH};$ENV{DATACHANNEL}/public)
    INCLUDE_DIRECTORIES($ENV{DATACHANNEL};$ENV{DATACHANNEL}/public)
ENDIF()



# CMAKE_CURRENT_SOURCE_DIR
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../public)
    GET_FILENAME_COMPONENT(PARENT_PATH ${CMAKE_CURRENT_SOURCE_DIR} PATH)
    # 设置自定义的 IDL 路径
    SET(CUSTOM_IDL_PATH ${PARENT_PATH}/public
                        ${CUSTOM_IDL_PATH})
ENDIF()

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../private)
    # 设置自定义的 IDL 路径
    GET_FILENAME_COMPONENT(PARENT_PATH ${CMAKE_CURRENT_SOURCE_DIR} PATH)
    SET(CUSTOM_IDL_PATH ${PARENT_PATH}/private
                        ${CUSTOM_IDL_PATH})
ENDIF()




# 注意：每次添加新的NEED_XXX依赖后，如果有需要链接新的依赖库，需要将最后的变量加入LINK_ALL_LIBS变量内。
# 设置所有的链接库
SET(LINK_ALL_LIBS   ${LINK_SDL2_LIBS})

MESSAGE(STATUS "SET LINK_ALL_LIBS = ${LINK_ALL_LIBS}")


####################################################################################################
MESSAGE(STATUS "\n**compile_IDL-----------------------------------------------------------------------")

MESSAGE(STATUS "GET CUSTOM_IDL_PATH = ${CUSTOM_IDL_PATH}")

## 检查是否有IDL文件需要编译 (支持指定多个IDL文件目录)
IF(DEFINED CUSTOM_IDL_PATH)
    # 注:为加快编译,public的目录下若已生成.h和.xpt文件，则不会再次编译idl文件.
    # 若要手动重新编译idl文件，请先执行`make clean`,然后执行 `cmake xxx`

    # 设置IDL文件搜索路径
    list(APPEND DEPS_IDL_PATH    "-I$ENV{APPCORE}/public")
    list(APPEND DEFAULT_IDL_PATH   "$ENV{APPCORE}/public")

    # DEFAULT_IDL_PATH 包含所有IDL文件目录，可以指定多个目录
    IF(DEFINED CUSTOM_IDL_PATH)
        SET(DEFAULT_IDL_PATH ${DEFAULT_IDL_PATH};${CUSTOM_IDL_PATH})
        MESSAGE(STATUS "SET DEFAULT_IDL_PATH = ${DEFAULT_IDL_PATH}")
    ENDIF()

    # 将这些目录添加到IDL文件的编译选项中，不用判断是否是相对路径
    FOREACH(each_deps_idl_path ${DEFAULT_IDL_PATH})
        list(APPEND DEPS_IDL_PATH "-I${each_deps_idl_path}")
    ENDFOREACH()

    # 去除重复编译选项
    list(REMOVE_DUPLICATES DEPS_IDL_PATH)
    list(REMOVE_DUPLICATES DEFAULT_IDL_PATH)
    MESSAGE(STATUS "GET DEPS_IDL_PATH = ${DEPS_IDL_PATH}")
    MESSAGE(STATUS "GET DEFAULT_IDL_PATH = ${DEFAULT_IDL_PATH}")

    # 设置IDL 可执行文件路径
    IF(WIN32)
        SET(XPIDL_NAME xpidl.exe)
        MESSAGE(STATUS "SET XPIDL_NAME = ${XPIDL_NAME}")
    ELSE()
        SET(XPIDL_NAME xpidl)
        MESSAGE(STATUS "SET XPIDL_NAME = ${XPIDL_NAME}")
    ENDIF()

    #编译idl文件
    IF((EXISTS ${ABDEPS}/xpidl/bin/${ABPLATFORM}/${CMAKE_BUILD_TYPE}/${XPIDL_NAME}))
        SET(XPIDL ${ABDEPS}/xpidl/bin/${ABPLATFORM}/${CMAKE_BUILD_TYPE}/${XPIDL_NAME})
        MESSAGE(STATUS "SET XPIDL = ${XPIDL}")

        IF(NOT WIN32)
            EXECUTE_PROCESS(COMMAND chmod +x ${XPIDL})
        ENDIF()
    ELSE()
        MESSAGE(WARNING "(LINE:${CMAKE_CURRENT_LIST_LINE}) Not found executable ${XPIDL_NAME} and ${XPTLINK_NAME}")
    ENDIF()

    MESSAGE(STATUS "Compile IDL files...")

    FOREACH(each_idl_path ${DEFAULT_IDL_PATH})
        MESSAGE(STATUS "GET each_idl_path = ${each_idl_path}")
        # 如果RELATIVE flag 指定，将会返回RELATIVE flag的相对路径
        # FILE(GLOB ...)返回的是绝对路径
        FILE(GLOB IDL_FILES "${each_idl_path}/*.idl")
        FILE(GLOB IDL_H_FILES "${each_idl_path}/*.h")
        IF(NOT "${IDL_FILES}" STREQUAL "")
            FOREACH(_in_file ${IDL_FILES})
                # 获取文件名和路径
                GET_FILENAME_COMPONENT(_out_file ${_in_file} NAME_WE)
                GET_FILENAME_COMPONENT(_in_PATH ${_in_file} PATH)
                GET_FILENAME_COMPONENT(_out_NAME ${_in_PATH} NAME)
                SET(_out_PATH ${PROJECT_BINARY_DIR}/${_out_NAME})
                file(MAKE_DIRECTORY ${_out_PATH})
                INCLUDE_DIRECTORIES(${_out_PATH})
                INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

                # 跟踪每个idl文件是否编译
                #MESSAGE(STATUS "COMMAND ${XPIDL} ${DEPS_IDL_PATH} ${_in_file} ...")
                ADD_CUSTOM_COMMAND(OUTPUT ${_out_PATH}/${_out_file}.h
                                COMMAND ${XPIDL}
                                ARGS ${DEPS_IDL_PATH} -m header -w -v -e ${_out_PATH}/${_out_file}.h  ${_in_file}
                                WORKING_DIRECTORY ${_in_PATH}
                                COMMENT ""
                                )

                LIST(APPEND IDL_HEADER_FILES ${_out_PATH}/${_out_file}.h)
                LIST(APPEND IDL_CLEAN_FILES ${_out_PATH}/${_out_file}.h)
            ENDFOREACH(_in_file)

            SET_SOURCE_FILES_PROPERTIES(${IDL_HEADER_FILES} PROPERTIES GENERATED TRUE)
            SET(${IDL_HEADER_FILES} ${IDL_HEADER_FILES})
        ENDIF()
    ENDFOREACH(each_idl_path)

    # success
    MESSAGE(STATUS "Compile IDL files SUCCESS...")

    # 清除变量
    UNSET(DEPS_IDL_PATH)
    UNSET(DEFAULT_IDL_PATH)
ENDIF()




####################################################################################################
MESSAGE(STATUS "\n**info-----------------------------------------------------------------------------")

# 配置是否自动执行生成的可执行文件
IF(AUTO_RUN)
   #MESSAGE(STATUS "enable auto_run...")
ENDIF()
MESSAGE(STATUS "GET AUTO_RUN = ${AUTO_RUN}")

# 输出一些build 信息(可选 )
MESSAGE(STATUS "GET CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "GET CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
MESSAGE(STATUS "GET CMAKE_SYSTEM_VERSION = ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "GET CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
MESSAGE(STATUS "GET PROJECT_NAME = ${PROJECT_NAME}")
MESSAGE(STATUS "GET CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
MESSAGE(STATUS "GET CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "GET TARGET_TYPE = ${TARGET_TYPE}")
MESSAGE(STATUS "GET CMAKE_BUILD_VERSION = ${CMAKE_BUILD_VERSION}")
MESSAGE(STATUS "GET PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "...")



####################################################################################################
MESSAGE(STATUS "\n**get_sourcefiles------------------------------------------------------------------")


# 获取当前目录及其子目录下的所有源文件
SET(SRC "")
FOREACH(subdir ${SOURCE_DIRS})
    IF(EXISTS ${subdir}/res)
        IF(NOT DEFINED RES_TARGET_NAME)
            SET(RES_TARGET_NAME "-")
        ENDIF()
        add_res_files_target(${subdir}/res ${LIBS_PATH} each_res_clean_files ${RES_TARGET_NAME})
    ENDIF()

    AUX_SOURCE_DIRECTORY(${subdir} subdir_src)

    MESSAGE(STATUS "GET subdir_src = ${subdir_src}")
    SET(SRC ${SRC} ${subdir_src})
    SET(RES_CLEAN_FILES ${RES_CLEAN_FILES} ${each_res_clean_files})
    SET(subdir_src "")
    SET(each_res_clean_files "")
ENDFOREACH(subdir ${DIRS})

SET(SOURCESRCS ${SRC})
#MESSAGE(STATUS "SET SOURCESRCS = ${SOURCESRCS}")



# 获取所有源文件,并统一处理成已绝对路径显示的文件名,以便进行文件过滤
SET(ALLSRCS ${SOURCESRCS} ${INCLUDE_SOURCE_FILES})
SET(ALL_SOURCES "")

FOREACH(_allsrc ${ALLSRCS})
    get_filename_component(_allsrc_path ${_allsrc} PATH)
    get_filename_component(_allsrc_name ${_allsrc} NAME)
    # _allsrc_path为空表示当前路径
    IF("${_allsrc_path}" STREQUAL "")
        SET(ALL_SOURCES ${ALL_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${_allsrc})
    ELSEIF("${_allsrc_path}" STREQUAL ".")
        SET(ALL_SOURCES ${ALL_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${_allsrc_name})
    ELSEIF(IS_ABSOLUTE ${_allsrc_path})
        SET(ALL_SOURCES ${ALL_SOURCES} ${_allsrc})
    ELSE()
        SET(ALL_SOURCES ${ALL_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${_allsrc})
    ENDIF()
ENDFOREACH()

#MESSAGE(STATUS "SET ALL_SOURCES = ${ALL_SOURCES}")




#过滤不需要编译的文件
IF("${EXCLUDE_SOURCE_FILES}" STREQUAL "")
    SET(OUTPUTS ${ALL_SOURCES}
                ${THRIFT_GEN_CPP_HDRS}
                ${THRIFT_GEN_CPP_SRCS}
                ${IDL_HEADER_FILES})
ELSE()
    SET(ALL_EXCLUDE_SOURCE_FILES "")
    FOREACH(_exclude_source_file ${EXCLUDE_SOURCE_FILES})
        get_filename_component(_exclude_source_file_path ${_exclude_source_file} PATH)
        get_filename_component(_exclude_source_file_name ${_exclude_source_file} NAME)
        # 为空表示当前路径
        IF("${_exclude_source_file_path}" STREQUAL "")
            SET(ALL_EXCLUDE_SOURCE_FILES ${ALL_EXCLUDE_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${_exclude_source_file})
        ELSEIF("${_exclude_source_file_path}" STREQUAL ".")
            set(ALL_EXCLUDE_SOURCE_FILES ${ALL_EXCLUDE_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${_exclude_source_file_name})
        ELSEIF(IS_ABSOLUTE ${_exclude_source_file_path})
            SET(ALL_EXCLUDE_SOURCE_FILES ${ALL_EXCLUDE_SOURCE_FILES} ${_exclude_source_file})
        ELSE()
            SET(ALL_EXCLUDE_SOURCE_FILES ${ALL_EXCLUDE_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${_exclude_source_file})
        ENDIF()
    ENDFOREACH()
    # 过滤不需要编译的文件，需要编译的文件放到变量OUTPUTS
    FILTER_OUT("${ALL_EXCLUDE_SOURCE_FILES}" "${ALL_SOURCES}" OUTPUTS)
    SET(OUTPUTS ${OUTPUTS}
                ${THRIFT_GEN_CPP_HDRS}
                ${THRIFT_GEN_CPP_SRCS}
                ${IDL_HEADER_FILES})
ENDIF()

MESSAGE(STATUS "SET ALL_EXCLUDE_SOURCE_FILES = ${ALL_EXCLUDE_SOURCE_FILES}")
MESSAGE(STATUS "SET OUTPUTS = ${OUTPUTS}")






####################################################################################################
MESSAGE(STATUS "\n**link_target----------------------------------------------------------------------")


#  链接
# 设置所有生成库和可执行文件的路径，在此统一所有输出路径。
# 设置不同的CMAKE_GENERATOR的生成目标路径(解决MSVC会自动创建Debug和Release目录)

# First for the generic no-config case (e.g. with mingw)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBS_PATH})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBS_PATH})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBS_PATH})
MESSAGE(STATUS "SET CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
MESSAGE(STATUS "SET CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
MESSAGE(STATUS "SET CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")


# Second, for multi-config builds (e.g. msvc)
FOREACH(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    MESSAGE(STATUS "GET OUTPUTCONFIG = ${OUTPUTCONFIG}")
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH})
ENDFOREACH(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


# link类型
MESSAGE(STATUS "GET TARGET_TYPE = ${TARGET_TYPE}")
IF("${TARGET_TYPE}" STREQUAL "RUNTIME")
    #如果是RUNTIME，则生成可执行文件
    # win32 windows程序需要加上WIN32选项
    IF(WIN32)
        IF("${SUBSYSTEM}" STREQUAL "WINDOWS")
            SET(EXPECT_LINK_FLAG WIN32)
            MESSAGE(STATUS "SET EXPECT_LINK_FLAG = ${EXPECT_LINK_FLAG}")
        ENDIF()
        IF(NOT MSVC_2015)
            SET(CUSTOM_LDFLAGS "${CUSTOM_LDFLAGS} /MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
            MESSAGE(STATUS "SET CUSTOM_LDFLAGS = ${CUSTOM_LDFLAGS}")
        ENDIF()
    ENDIF()

    #如果是RUNTIME,则生成可执行文件
    LINK_DIRECTORIES(${LIBS_PATH})
    ADD_EXECUTABLE(${TARGET_NAME} ${EXPECT_LINK_FLAG} ${OUTPUTS})
    TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})
    MESSAGE(STATUS "TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})")

    #RUNTIME
    install_target(RUNTIME ${TARGET_NAME} .)

    #debuginfo
    MESSAGE(STATUS "GET NEED_DEBUGINFO = ${NEED_DEBUGINFO}")
    IF(NEED_DEBUGINFO)
        SET(TARGET_FULL_NAME ${TARGET_NAME})
        ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}
                           COMMAND objcopy --only-keep-debug ${TARGET_FULL_NAME} ${TARGET_FULL_NAME}.debug
                           COMMAND objcopy --strip-debug ${TARGET_FULL_NAME}
                           COMMAND objcopy --add-gnu-debuglink=${TARGET_FULL_NAME}.debug ${TARGET_FULL_NAME}
                           WORKING_DIRECTORY ${LIBS_PATH})
        LIST(APPEND DEBUGINFO_CLEAN_FILES ${LIBS_PATH}/${TARGET_FULL_NAME}.debug)
        install_files(debuginfo ${LIBS_PATH}/${TARGET_FULL_NAME}.debug)
    ENDIF()


    #判断是否自动执行，由AUTO_RUN来开启和关闭
    IF(WIN32)
        SET(RUN_NAME ${TARGET_NAME}.exe)
        IF(AUTO_RUN)
            ADD_CUSTOM_TARGET(RUN_${TARGET_NAME} ALL
                                COMMAND start /WAIT ${RUN_NAME}
                                DEPENDS ${TARGET_NAME}
                                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                                COMMENT "running ${RUN_NAME} ... "
                    )
        ELSE()
            ADD_CUSTOM_TARGET(RUN_${TARGET_NAME}
                                COMMAND start /WAIT ${RUN_NAME}
                                DEPENDS ${TARGET_NAME}
                                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                                COMMENT "running ${RUN_NAME} ... "
                    )
        ENDIF()
    ELSE()
        SET(RUN_NAME ${TARGET_NAME})
        IF(AUTO_RUN)
            ADD_CUSTOM_TARGET(RUN_${TARGET_NAME} ALL
                                COMMAND ${RUN_NAME}
                                DEPENDS ${TARGET_NAME}
                                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                                COMMENT "running ${RUN_NAME} ... "
                    )
        ELSE()
            ADD_CUSTOM_TARGET(RUN_${TARGET_NAME}
                                COMMAND ${RUN_NAME}
                                DEPENDS ${TARGET_NAME}
                                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                                COMMENT "running ${RUN_NAME} ... "
                    )
        ENDIF()
    ENDIF()

    #代码覆盖率
    MESSAGE(STATUS "GET ENABLE_GCOV = ${ENABLE_GCOV}")
    IF (ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
        INCLUDE(EnableCoverageReport)
        SET(CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
        MESSAGE(STATUS "SET CURRENT_PATH = ${CURRENT_PATH}")

        GET_FILENAME_COMPONENT(SRC_NAME ${CURRENT_PATH} NAME)

        WHILE(NOT ${SRC_NAME} STREQUAL "test")
            GET_FILENAME_COMPONENT(PARENT_PATH ${CURRENT_PATH} PATH)
            GET_FILENAME_COMPONENT(SRC_NAME ${PARENT_PATH} NAME)
            SET(CURRENT_PATH ${PARENT_PATH})
        ENDWHILE()

        GET_FILENAME_COMPONENT(COVERAGE_SOURCE_DIR ${CURRENT_PATH} PATH)
        GET_FILENAME_COMPONENT(COVERAGE_SOURCE_NAME ${COVERAGE_SOURCE_DIR} NAME)
        GET_FILENAME_COMPONENT(PARENT_PATH ${CMAKE_CURRENT_SOURCE_DIR} PATH)

        ENABLE_COVERAGE_REPORT(TARGETS ${TARGET_NAME} FILTER "*deps*;/usr/include/*;/usr/lib/*")
    ENDIF()

    #性能分析
    MESSAGE(STATUS "GET ENABLE_PROFILE = ${ENABLE_PROFILE}")
    IF(ENABLE_PROFILE AND WIN32)
        SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES LINK_FLAGS /PROFILE)
    ENDIF()

ELSEIF("${TARGET_TYPE}" STREQUAL "LIBRARY")
    # 链接成一个动态库
    LINK_DIRECTORIES(${LIBS_PATH})
    ADD_LIBRARY(${TARGET_NAME} SHARED ${OUTPUTS})
    TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})
    MESSAGE(STATUS "TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})")

    #LIBRARY
    IF(WIN32)
        # Windows下的动态库被认为是RUNTIME类型
        install_target(RUNTIME ${TARGET_NAME} .)
        install_target(ARCHIVE ${TARGET_NAME} .)
    ELSE()
        install_target(LIBRARY ${TARGET_NAME} .)
    ENDIF()

    #debuginfo
    MESSAGE(STATUS "GET NEED_DEBUGINFO = ${NEED_DEBUGINFO}")
    IF(NEED_DEBUGINFO)
        SET(TARGET_FULL_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${TARGET_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}
                           COMMAND objcopy --only-keep-debug ${TARGET_FULL_NAME} ${TARGET_FULL_NAME}.debug
                           COMMAND objcopy --strip-debug ${TARGET_FULL_NAME}
                           COMMAND objcopy --add-gnu-debuglink=${TARGET_FULL_NAME}.debug ${TARGET_FULL_NAME}
                           WORKING_DIRECTORY ${LIBS_PATH})
        LIST(APPEND DEBUGINFO_CLEAN_FILES ${LIBS_PATH}/${TARGET_FULL_NAME}.debug)
        install_files(debuginfo ${LIBS_PATH}/${TARGET_FULL_NAME}.debug)
    ENDIF()
ELSEIF("${TARGET_TYPE}" STREQUAL "ARCHIVE")
    # 链接成一个静态库
    LINK_DIRECTORIES(${LIBS_PATH})
    ADD_LIBRARY(${TARGET_NAME} STATIC ${OUTPUTS})
    TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})
    MESSAGE(STATUS "TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})")
    install_target(ARCHIVE ${TARGET_NAME} .)
ELSEIF("${TARGET_TYPE}" STREQUAL "COMPONENT")
    # 链接成一个组件库
    #设置生成目标为组件的特别处理，存放位置为${LIBS_PATH}/components

    # First for the generic no-config case (e.g. with mingw)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBS_PATH}/components)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBS_PATH}/components)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBS_PATH}/components)

    # Second, for multi-config builds (e.g. msvc)
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH}/components)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH}/components)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBS_PATH}/components)
    endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

    #如果是COMPONENT，则生成动态库
    LINK_DIRECTORIES(${LIBS_PATH})
    ADD_LIBRARY(${TARGET_NAME} SHARED ${OUTPUTS})
    TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})
    MESSAGE(STATUS "TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LINK_ALL_LIBS})")

    # 将组件库install到components目录
    IF(WIN32)
        install_target(RUNTIME ${TARGET_NAME} components)
    ELSE()
        install_target(LIBRARY ${TARGET_NAME} components)
    ENDIF()

    #debuginfo
    MESSAGE(STATUS "GET NEED_DEBUGINFO = ${NEED_DEBUGINFO}")
    IF(NEED_DEBUGINFO)
        SET(TARGET_FULL_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${TARGET_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}
                           COMMAND objcopy --only-keep-debug ${TARGET_FULL_NAME} ${TARGET_FULL_NAME}.debug
                           COMMAND objcopy --strip-debug ${TARGET_FULL_NAME}
                           COMMAND objcopy --add-gnu-debuglink=${TARGET_FULL_NAME}.debug ${TARGET_FULL_NAME}
                           WORKING_DIRECTORY ${LIBS_PATH}/components)
        LIST(APPEND DEBUGINFO_CLEAN_FILES ${LIBS_PATH}/components/${TARGET_FULL_NAME}.debug)
        install_files(debuginfo/components ${LIBS_PATH}/components/${TARGET_FULL_NAME}.debug)
    ENDIF()

ELSEIF("${TARGET_TYPE}" STREQUAL "")
    #如果没有指定，默认尝试生成动态库
    MESSAGE(WARNING "(LINE:${CMAKE_CURRENT_LIST_LINE}) No giving generation type, try to create dynamic library")
    INCLUDE(link_library)
ENDIF()






# 是否设置RES_FILES
MESSAGE(STATUS "GET RES_FILES = ${RES_FILES}")
IF(NOT "${RES_FILES}" STREQUAL "")
    ADD_CUSTOM_TARGET(${TARGET_NAME}_res ALL DEPENDS ${RES_FILES})
    ADD_DEPENDENCIES(${TARGET_NAME} ${TARGET_NAME}_res)

    MESSAGE(STATUS "ADD_DEPENDENCIES(${TARGET_NAME})")
    MESSAGE(STATUS "ADD_DEPENDENCIES(${TARGET_NAME}_res)")
ENDIF()



# 是否设置target版本
MESSAGE(STATUS "GET TARGET_VERSION = ${TARGET_VERSION}")
IF(NOT "${TARGET_VERSION}" STREQUAL "")
    SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES VERSION ${TARGET_VERSION})
ENDIF()


# 自定义链接参数
MESSAGE(STATUS "GET CUSTOM_LDFLAGS = ${CUSTOM_LDFLAGS}")
IF (DEFINED CUSTOM_LDFLAGS)
    SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES LINK_FLAGS ${CUSTOM_LDFLAGS})
ENDIF ()





####################################################################################################
MESSAGE(STATUS "\n**custom_target----------------------------------------------------------------------")

# 添加自定义命令
MESSAGE(STATUS "GET ENABLE_TARGET = ${ENABLE_TARGET}")

IF(${ENABLE_TARGET})
    #每个命令只能执行一次，慎用
    # add_custom_target(cleanall ...),用来添加target cleanall
    IF(WIN32)
        SET(MAKE_FLAGS /NOLOGO)
        MESSAGE(STATUS "SET MAKE_FLAGS = ${MAKE_FLAGS}")
    ENDIF()


    ADD_CUSTOM_TARGET(cleanall
                        COMMAND ${CMAKE_BUILD_TOOL} ${MAKE_FLAGS} clean
                        COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/CMakeCache.txt
                        COMMENT "Execute cleaning work, delete CMakeCache files !"
    )
    MESSAGE(STATUS "COMMAND ${CMAKE_BUILD_TOOL} ${MAKE_FLAGS} clean")
    MESSAGE(STATUS "COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/CMakeCache.txt")
    MESSAGE(STATUS "COMMENT Execute cleaning work, delete CMakeCache files !")


    # add_custom_target(cleancache ...),用来添加target cleancache
    ADD_CUSTOM_TARGET(cleancache
                        COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/CMakeCache.txt
                        COMMENT "delete CMakeCache files !"
    )
    MESSAGE(STATUS "COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/CMakeCache.txt")
    MESSAGE(STATUS "COMMENT delete CMakeCache files !")
ENDIF()







####################################################################################################
MESSAGE(STATUS "\n**add make clean-------------------------------------------------------------------")


# do additional `make clean`
# 添加编译IDL文件后的文件、mo文件和到`make clean`选项，以便`make clean `清除
SET(ADDITIONAL_CLEAN_FILES ${RES_CLEAN_FILES};
                           ${IDL_CLEAN_FILES};
                           ${THRIFT_CLEAN_FILES};
                           ${DEBUGINFO_CLEAN_FILES})
MESSAGE(STATUS "SET ADDITIONAL_CLEAN_FILES = ${ADDITIONAL_CLEAN_FILES}")

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ADDITIONAL_CLEAN_FILES}")




####################################################################################################
MESSAGE(STATUS "\n**package-------------------------------------------------------------------------")

# 打包
MESSAGE(STATUS "GET ENABLE_PACKAGE = ${ENABLE_PACKAGE}")
IF(ENABLE_PACKAGE)
    IF(WIN32)
        SET(CPACK_OUTPUT_FILE_PREFIX ${PACKAGE_PATH})
        # 设置打包格式
        SET(CPACK_SOURCE_GENERATOR "ZIP")
        SET(CPACK_GENERATOR "ZIP")
    ELSE()
        SET(CPACK_OUTPUT_FILE_PREFIX ${PACKAGE_PATH})
        # 设置打包格式
        SET(CPACK_SOURCE_GENERATOR "TGZ")
        SET(CPACK_GENERATOR "TGZ")
    ENDIF()
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CMAKE_PROJECT_NAME})

    MESSAGE(STATUS "SET CPACK_OUTPUT_FILE_PREFIX = ${CPACK_OUTPUT_FILE_PREFIX}")
    MESSAGE(STATUS "SET CPACK_SOURCE_GENERATOR = ${CPACK_SOURCE_GENERATOR}")
    MESSAGE(STATUS "SET CPACK_GENERATOR = ${CPACK_GENERATOR}")
    MESSAGE(STATUS "SET CPACK_DEBIAN_PACKAGE_MAINTAINER = ${CPACK_DEBIAN_PACKAGE_MAINTAINER}")

    # 获取时间
    TODAY(tdate)
    MESSAGE(STATUS "GET tdate = ${tdate}")

    # 设置打包的版本号和版本
    # package
    MESSAGE(STATUS "GET CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}")
    MESSAGE(STATUS "GET CPACK_PACKAGE_NAME = ${CPACK_PACKAGE_NAME}")
    MESSAGE(STATUS "GET CPACK_PACKAGE_VERSION = ${CPACK_PACKAGE_VERSION}")
    MESSAGE(STATUS "GET CPACK_PACKAGE_TYPE = ${CPACK_PACKAGE_TYPE}")
    IF(DEFINED CPACK_PACKAGE_FILE_NAME)
        # 如果定义CPACK_PACKAGE_FILE_NAME,nothing to do

    ELSEIF((DEFINED CPACK_PACKAGE_NAME) AND (DEFINED CPACK_PACKAGE_VERSION))
            IF(DEFINED CPACK_PACKAGE_TYPE)
                SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${tdate}-${CPACK_PACKAGE_TYPE})
                MESSAGE(STATUS "SET CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}")
            ELSE()
                SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${tdate}-${CMAKE_BUILD_TYPE})
                MESSAGE(STATUS "SET CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}")
            ENDIF()
    ELSE()
        #to do or not to do,define the package name ?
    ENDIF()

    #source package
    MESSAGE(STATUS "GET CPACK_SOURCE_PACKAGE_NAME = ${CPACK_SOURCE_PACKAGE_NAME}")
    IF(DEFINED CPACK_PACKAGE_FILE_NAME)
        # nothing to do
    ELSEIF((DEFINED CPACK_SOURCE_PACKAGE_NAME) AND (DEFINED CPACK_PACKAGE_VERSION))
        IF(DEFINED CPACK_PACKAGE_TYPE)
            SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_SOURCE_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${tdate}-${CMAKE_BUILD_TYPE})
            MESSAGE(STATUS "SET CPACK_SOURCE_PACKAGE_FILE_NAME = ${CPACK_SOURCE_PACKAGE_FILE_NAME}")
        ELSE()
            SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_SOURCE_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${tdate}-${CPACK_PACKAGE_TYPE})
            MESSAGE(STATUS "SET CPACK_SOURCE_PACKAGE_FILE_NAME = ${CPACK_SOURCE_PACKAGE_FILE_NAME}")
        ENDIF()
    ELSE()
        #to do or not to do,define the package_source name ?
    ENDIF()

    # INCLUDE应该放在最后
    #C:\Program Files\CMake\share\cmake-3.7\Modules\CPack.cmake
    INCLUDE(CPack)
ENDIF()


####################################################################################################
MESSAGE(STATUS "\n**target.cmake end----------------------------------------------------------------")

MESSAGE(STATUS "GET CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
MESSAGE(STATUS "GET CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
MESSAGE(STATUS "GET CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "GET CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
MESSAGE(STATUS "GET CMAKE_EXE_LINKER_FLAGS_RELEASE = ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
MESSAGE(STATUS "GET CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
MESSAGE(STATUS "GET CMAKE_SHARED_LINKER_FLAGS_RELEASE = ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
MESSAGE(STATUS "GET CMAKE_STATIC_LINKER_FLAGS = ${CMAKE_STATIC_LINKER_FLAGS}")

####################################################################################################
MESSAGE(STATUS "\n**target.cmake end----------------------------------------------------------------")
MESSAGE(STATUS "")
MESSAGE(STATUS "")





