#!/bin/bash

# 因为脚本内容用的都是相对路径, 一下命令让pwd返回tool目录
if [ "${MYCMAKE}" != "" ]
then
    cd "${MYCMAKE}/tool"
fi

OSNAME=`uname -s`

# 根据操作系统信息加载对应的变量文件
case $OSNAME in
    Darwin)     #OSX
        KERNEL_VERSION=`uname -r`
        PROCESSOR=`uname -m`
        case "$KERNEL_VERSION" in
            *20*)
                source ./Darwin_x64_env.sh
                ;;
            *)
                echo NOT_SUPPORT_VERSION
                ;;
        esac
        ;;
    Linux)
        KERNEL_VERSION=`uname -r`
        PROCESSOR=`uname -m`
        case "$KERNEL_VERSION" in
            *.el7.*)
        echo here
                source ./Linux_el7_x64_env.sh
                ;;
            2.6.24*|2.6.18*)
                case "$PROCESSOR" in
                    x86_64)
                        case $1 in
                            *86)
                                source ./Redhat_5_x86_env.sh
                                ;;
                            *)
                                source ./Redhat_5_x64_env.sh
                                ;;
                        esac
                        ;;
                    *86)
                        source ./Redhat_5_x86_env.sh
                        ;;
                esac
                ;;
            *)
                if [ "$CMAKE_PLATFORM_NAME" == "" ] || [ "$CMAKE_PLATFORM_VERSION" = "" ]
                then
                    echo
                    echo WARNING: CMAKE_PLATFORM_NAME OR CMAKE_PLATFORM_VERSION NOT SET
                    echo Please check and set proper value
                    echo
                fi
                ;;
        esac
        ;;
    *)
        echo
        echo NOT_SUPPORT_SYSTEM
        echo
        ;;
esac
