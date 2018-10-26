#!/bin/bash

# 设定PROGRAM变量和当前目录变量
cd ../..
MYPROJECT=`pwd`
cd -
OLD_PATH=`pwd`

# 设定CMAKE变量
CMAKE_PLATFORM_NAME=Redhat
CMAKE_PLATFORM_VERSION=5
CMAKE_BUILD_VERSION=x86
CMAKE_CXX_COMPILER=/usr/local/gcc-4.9.2/bin/g++
CMAKE_C_COMPILER=/usr/local/gcc-4.9.2/bin/gcc

# 加载环境变量设置脚本
source ./setEnv.sh

# 设置环境变量
setEnvs
