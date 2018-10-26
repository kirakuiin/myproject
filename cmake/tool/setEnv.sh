#!/bin/bash

#
# 检查环境变量是否存在
#
function checkEnvExist()
{
    # 使用正则表达式在环境变量之中搜索参数是否存在并不输出结果
    # eval会对表达式进行两次扫瞄, 假设参数为SRC, 则第一次扫描结果为[ -z "$SRC" ]
    # 第二次执行[ -z "$SRC" ], 其中-z测试字符串长度是否为0, 而[]作为test表达式会
    # 返回0和1, 如果字符串长度为0这表达式为真, 输出null, 否则输出环境变量的值
    if set | grep -E "^$1=">/dev/null
    then eval [ -z "\$$1" ] && echo null || eval echo \$$1
    else echo unset
    fi
}
#
# 设置编译需要的环境变量
#
function setEnvs()
{
    #
    # cmake 环境变量
    #
    export CMAKE_PLATFORM_NAME      # 平台名称 RedHat, Windows
    export CMAKE_PLATFORM_VERSION   # 平台版本 5, 10
    export CMAKE_BUILD_VERSION      # cpu架构 x86, x64
    export CMAKE_CXX_COMPILER       # C++编译器路径
    export CMAKE_C_COMPILER         # C编译器路径

    #
    # 构建相关路径
    #
    export MYPLATFORM=${CMAKE_PLATFORM_NAME}_${CMAKE_PLATFORM_VERSION}_${CMAKE_BUILD_VERSION}
    export MYSRC=$MYPROJECT/src
    export MYDEPS=$MYPROJECT/deps
    export MYCMAKE=$MYPROJECT/cmake
    export MYTARGET=$MYPROJECT/target/$MYPLATFORM/libs
    export MYBUILD=$MYPROJECT/target/$MYPLATFORM/build
    export MYPROJECT=$MYPROJECT

    #
    # 将tool目录加入环境变量
    #
    export PATH=$PATH:$MYPROJECT/cmake/tool

    #
    # 为所有脚本添加可执行权限
    #
    chmod -R +x $MYPROJECT/cmake/tool/*.sh

    #
    # 输出变量设置结果
    #
    echo "****************************************************************"
    echo "MYSRC is                   $MYSRC"
    echo "MYDEPS is                  $MYDEPS"
    echo "MYCMAKE is                 $MYCMAKE"
    echo "MYBUILD is                 $MYBUILD"
    echo "MYTARGET is                $MYTARGET"
    echo "CMAKE_PLATFORM_NAME is     $CMAKE_PLATFORM_NAME"
    echo "CMAKE_PLATFORM_VERSION is  $CMAKE_PLATFORM_VERSION"
    echo "CMAKE_BUILD_VERSION is     $CMAKE_BUILD_VERSION"
    echo "****************************************************************"

    echo "Go to \$MYSRC"

    alias makec='makec.sh'

    cd $MYSRC
}
