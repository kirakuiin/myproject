编译方法:
       cmake .
       make
       make DESTDIR=~/.../lib/ install

如果出现源代码文件格式无法识别，修改为utf-8即可
如果库的依赖路径为绝对路径(osx下):
    使用install_name_tool -id {old_path} filename 来修改自身依赖
    使用install_name_tool -change {old_path} {new_path} filename 来修改第三方依赖
