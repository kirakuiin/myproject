# -*- encoding: utf-8 -*-
# author: wang.zhuowei@foxmail.com
# date: Oct 29, 2018
# license: GPL.v3

"""使用Google Translate进行语言转换

这段代码并不是单独作为一个程序运行, 而是作为Alfred的一个组件完成调用Google
翻译返回结果这一比较重要的工作。

"""

# pylint: disable=invalid-name

import sys

from google_api import TranslateRequest, GoogleTranslate as Trans


def parse_argv(args: list) -> TranslateRequest:
    """解析输入的命令行参数

    命令行参数的开头和结尾存在s_开头的字符串(到空格结束), 则将其解释为:
        源语言, 如果未识别到则为默认值
    命令行参数的开头和结尾存在t_开头的字符串(到空格结束), 则将其解释为:
        目标语言, 如果未识别到则为默认值
    其余部分被解释为查询文本(即使不连续也会拼接)

    argv: 命令行参数
    return: TranslateOption对象

    >>> parse_argv(['python', 's_ja', 'hello', 't_en', 'world'])
    TranslateRequest(query='hello world', s_lang='ja', t_lang='en')
    >>> parse_argv(['python', 't_ja', 'hello', 'world'])
    TranslateRequest(query='hello world', s_lang='auto', t_lang='ja')

    """
    def set_lang(request: TranslateRequest, arg: str) -> bool:
        """设置语言来源和目的, 如果成功则返回真"""
        result = True
        if arg.startswith('s_') and Trans.is_valid_lang(arg[2:]):
            request.s_lang = arg[2:]
        elif arg.startswith('t_') and Trans.is_valid_lang(arg[2:]):
            request.t_lang = arg[2:]
        else:
            result = False
        return result

    request = TranslateRequest('')
    argv_list = args[1:]    # 排除脚本名称
    argv_list.extend(argv_list[0].split(' '))   # 如果第一个元素包含空格, 拆分
    argv_list = argv_list[1:]   # 去除第一个重复元素

    query_list = []
    for arg in argv_list:
        if set_lang(request, arg):
            pass
        else:
            query_list.append(arg)

    request.query = ' '.join(query_list)
    return request


def main():
    """主函数, 负责处理输入和输出"""
    argv = sys.argv
    req = parse_argv(argv)
    exector = Trans()
    result = exector.translate(req)
    print(result.trans_text)


if __name__ == '__main__':
    main()
