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

from google_api import (TranslateRequest, GoogleTranslate as Trans,
                        TranslateResult)
from workflow import (WorkflowItem, WorkflowList, IconSubItem, ModsSubItem)


def parse_argv(args: list = None) -> TranslateRequest:
    """解析输入的命令行参数

    命令行参数的开头和结尾存在s_开头的字符串(到空格结束), 则将其解释为:
        源语言, 如果未识别到则为默认值
    命令行参数的开头和结尾存在t_开头的字符串(到空格结束), 则将其解释为:
        目标语言, 如果未识别到则为默认值
    其余部分被解释为查询文本(即使不连续也会拼接)

    argv: 命令行参数
    return: TranslateOption对象

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


    if args is None:
        args = sys.argv
    argv_list = args[1:]    # 排除脚本名称
    argv_list.extend(argv_list[0].split(' '))   # 如果第一个元素包含空格, 拆分
    argv_list = argv_list[1:]   # 去除第一个重复元素

    query_list = []
    request = TranslateRequest('')
    for arg in argv_list:
        if set_lang(request, arg):
            pass
        else:
            query_list.append(arg)

    request.query = ' '.join(query_list)
    return request


def handle_output(trans_res: TranslateResult):
    """处理翻译结果

    将翻译结果转换为Alfred可以接受的形式
    """
    wf_list = WorkflowList()

    icon_item = IconSubItem(path='result.png') # 图标
    cmd_sub_item = ModsSubItem.CmdSubSubItem(subtitle=trans_res.s_transcription)
    alt_sub_item = ModsSubItem.AltSubSubItem(subtitle=trans_res.t_transcription)
    ctrl_url = '{url}/#{s_lang}/{t_lang}/{text}'.format(
        url=Trans.GOOGLE_TRANSLATE_URL, text=trans_res.origin_text,
        s_lang=trans_res.s_lang, t_lang=trans_res.t_lang
    )
    ctrl_sub_item = ModsSubItem.CtrlSubSubItem(subtitle='进入Google翻译页面',
                                               arg=ctrl_url, web=True)
    mods_item = ModsSubItem(alt=alt_sub_item, cmd=cmd_sub_item,
                            ctrl=ctrl_sub_item) # 可选条目
    trans_item = WorkflowItem(title=trans_res.trans_text, icon=icon_item,
                              subtitle='翻译结果', mods=mods_item,)
    wf_list.add_item(trans_item)

    icon_item = IconSubItem(path='dict.png')
    dict_list = []
    for word in trans_res.dictionary:
        title = '[{}] {}'.format(word.type, word.text)
        dict_list.append(title)
        other_item = WorkflowItem(title=title, subtitle=word.synonym,
                                  icon=icon_item)
        wf_list.add_item(other_item)

    wf_list.add_variables(word=trans_res.origin_text, s_l=trans_res.s_lang,
                          t_l=trans_res.t_lang, meaning=trans_res.trans_text,
                          dict_def=';'.join(dict_list))

    wf_list.output_to_alfred()


def main():
    """主函数, 负责处理输入和输出"""
    req = parse_argv()
    exector = Trans()
    result = exector.translate(req)
    handle_output(result)


if __name__ == '__main__':
    main()
