编译方法:
    1. 下载boost安装包 www.boost.org
    2. 运行./bootstrap.sh --prefix=/xxx
    3. 运行.b2 /tools/bcp, 编译裁剪工具
    4. dist/bin/bcp boost-build.jam boostcpp.jam bootstrap.sh build [output_path],
    这一步的目的是将编译工具放到output_path里
    5. dist/bin/bcp asio system filesystem thread format timer date_time flyweight pool uuid unordered property_tree [output_path]
    这一步是将需要的库放入output_path里
    6. cd [output_path] && ./bootstrap && ./b2 install 完成编译

如果想要编译为一个文件, 在[output_path]内写一个cmakelists.txt, 手动编译
