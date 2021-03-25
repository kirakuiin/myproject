#!/bin/bash

# 设定PROGRAM变量和当前目录变量
cd ../..
MYPROJECT=`pwd`
cd -
OLD_PATH=`pwd`

# 设定CMAKE变量
CMAKE_PLATFORM_NAME=Darwin
CMAKE_PLATFORM_VERSION=20
CMAKE_BUILD_VERSION=x64
CMAKE_CXX_COMPILER=/usr/bin/g++
CMAKE_C_COMPILER=/usr/bin/gcc

# 加载环境变量设置脚本
source ./setEnv.sh

# 设置环境变量
setEnvs
