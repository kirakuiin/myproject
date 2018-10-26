// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-30

#include <string>
#include <iostream>
#include <regex>

using std::string;
using std::cout;
using std::endl;

int main()
{
    std::regex pat(R"(.+?\[(\d+)\])");
    std::smatch result;
    const string str1 = "/a/b/c[1][2][3]<4>";
    std::regex_search(str1, result, pat);
    cout<<result[0].str()<<endl;
    cout<<result[1].str()<<endl;
    cout<<result[2].str()<<endl;
    return 0;
}
