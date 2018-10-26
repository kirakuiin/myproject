常用运行参数选择：
一、对执行的测试案例进行过滤，支持通配符

--gtest_filter

TEST(testsuitname, testcasename)
TEST_F(testsuitname, testcasename)

?    单个字符
*    任意字符
-    排除，如，-a 表示除了a
:    取或，如，a:b 表示a或b

比如下面的例子：
./gtestdemo0 
没有指定过滤条件，运行所有用例

./gtestdemo0 --gtest_filter=* 
使用通配符*，表示运行所有用例

./gtestdemo0 --gtest_filter=googletest1.* 
运行所有testsuitname 为 googletest1的所有用例

./gtestdemo0 --gtest_filter=googletest1.*:googletest11.* 
运行所有testsuitname 为googletest1和googletest11的所有用例

./gtestdemo0 --gtest_filter=googletest11.aaa* 
运行与testsuitname 为googletest11，且testcasename前面三个字母为aaa的匹配的所有用例

./gtestdemo0 --gtest_filter=googletest11.aaa*-googletest11.aaabbb 
运行所有上述所有用例，但是除了googletest11.aaabbb这个案例




二、将测试结果输出到一个xml中
--gtest_output=xml[:DIRECTORY_PATH\|:FILE_PATH]

--gtest_output=xml
不指定输出路径时，默认为当前路径，文件名为test_detail.xml

--gtest_output=xml:d:\ 
指定输出到某个目录的test_detail.xml文件中

--gtest_output=xml:d:\foo.xml
指定输出到d:\foo.xml




三、其他参考资料待整理

运行参数的使用，请看

http://www.cnblogs.com/coderzh/archive/2009/04/10/1432789.html


测试用例的setup，teardown等行为的使用请看

http://www.cnblogs.com/coderzh/archive/2009/04/06/1430396.html


各种断言请看

http://www.cnblogs.com/coderzh/archive/2009/04/06/1430364.html
