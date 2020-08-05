irrklang不提供源代码，只能使用官方提供的库
通过install_name_tool -id @rpath/irrklang.dylib irrklang.dylib这个命令将
动态库依赖修改为动态路径
