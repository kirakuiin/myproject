# -*- encoding: utf-8 -*-
# author: wang.zhuowei@foxmail.com
# date: Nov 1, 2018
# license: GPL.v3

"""封装对Alfred的传输协议

主要的作用时把程序输出的一般对象转化为Alfred可以接受的json对象
"""

# pylint: disable=invalid-name, too-many-instance-attributes, line-too-long

import sys
import json


__all__ = ['IconSubItem', 'TextSubItem', 'ModsSubItem',
           'WorkflowItem', 'WorkflowList']


class BaseItem:
    """Alfred所有属性对象的基类

    提供两个实用接口方便子类使用
    """

    def get_json_repr(self) -> dict:
        """获得字符串的json形式的表述

        return: 一个显示各个属性的dict对象

        """
        jsexpr = {}
        for attr, value in self.__dict__.items():
            if value and not attr.startswith('_'):
                try:
                    jsexpr[attr] = value.get_json_repr()
                except AttributeError:
                    jsexpr[attr] = value

        return jsexpr

    def __repr__(self):
        cls_name = type(self).__name__
        args_str = []
        for attr, value in self.__dict__.items():
            if value and not attr.startswith('_'):
                if isinstance(value, str):
                    value = "'{}'".format(value)
                args_str.append("{}={}".format(attr, value))
        args_str = ', '.join(args_str)
        if len(args_str) > 30:
            args_str = args_str[:27] + '...'

        return '{}({})'.format(cls_name, args_str)


class IconSubItem(BaseItem):
    """Alfred图标子对象"""

    # pylint: disable=redefined-builtin
    def __init__(self, path, type=None):
        self.path = path    # 图标的路径
        self.type = type    # 图标的类型


class TextSubItem(BaseItem):
    """Alfred文本子对象"""

    def __init__(self, copy, largetype=None):
        self.copy = copy            # 拷贝到粘贴板里的信息
        self.largetype = largetype  # 放大显示的信息


class ModsSubItem(BaseItem):
    """Alfred模块子对象

    目前支持alt, cmd, ctrl. 分别代表了键盘左下方的三个功能键触发对应的功能

    >>> alt = ModsSubItem.AltSubSubItem('alt', var='alt')
    >>> cmd = ModsSubItem.CmdSubSubItem('cmd', var='cmd')
    >>> ctrl = ModsSubItem.CtrlSubSubItem('ctrl', var='ctrl')
    >>> mod_item = ModsSubItem(alt=alt, cmd=cmd, ctrl=ctrl)
    >>> mod_item.get_json_repr() #doctest: +ELLIPSIS
    {'alt': {'subtitle': 'alt', 'valid': True, 'arg': 'alt', 'variables': {'var': 'alt'}}, ...}
    """

    class ModsSubSubItem(BaseItem):
        """隶属于ModsSubItem的子属性"""

        def __init__(self, subtitle, *, arg=None, valid=True, **kwargs):
            self.subtitle = subtitle
            self.valid = valid
            if arg is None:
                arg = subtitle
            self.arg = arg
            self.variables = kwargs

    class AltSubSubItem(ModsSubSubItem):
        """Alt属性"""
        pass

    class CmdSubSubItem(ModsSubSubItem):
        """Cmd属性"""
        pass

    class CtrlSubSubItem(ModsSubSubItem):
        """Ctrl属性"""
        pass

    def __init__(self, *, alt, cmd, ctrl):
        self.alt = alt  # 按住alt键时显示的信息
        self.cmd = cmd  # 按住cmd键时显示的信息
        self.ctrl = ctrl # 按住ctrl键时显示的信息


class WorkflowItem(BaseItem):
    """Alfred行对象

    这个对象表示Alfred中显示结果中一行这一概念
    参考: https://www.alfredapp.com/help/workflows/inputs/script-filter/json/

    >>> item = WorkflowItem('hi', subtitle='halo')
    >>> item
    WorkflowItem(title='hi', subtitle='halo'...)
    >>> icon_item = IconSubItem('/usr/bin/flow.png')
    >>> item.icon = icon_item
    >>> item.get_json_repr()
    {'title': 'hi', 'subtitle': 'halo', 'arg': 'hi', 'valid': True, 'icon': {'path': '/usr/bin/flow.png'}, 'uid': '0'}

    """

    _base_uid = 0

    # pylint: disable=too-many-arguments
    def __init__(self, title, *, subtitle=None, arg=None, valid=True,
                 autocomplete=None, quicklookurl=None, icon=None,
                 mods=None, text=None, **variables):
        self.title = title                      # 标题
        self.subtitle = subtitle                # 副标题
        if arg is None:
            arg = title
        self.arg = arg                          # 传递给下一个组件的参数
        self.valid = valid                      # 是否可以被选中
        self.autocomplete = autocomplete        # 自动补全
        self.quicklookurl = quicklookurl        # 快速查看的URL[cmd+Y]
        self.icon = icon    # 图标属性
        self.mods = mods    # 模组属性(按住alt或者cmd时展示的信息)
        self.text = text    # 文本属性(粘贴板[cmd+C]和字符放大[cmd+L])
        self.variables = variables  # 全局变量

        self.uid = str(type(self)._base_uid)    # 唯一标识uid
        type(self)._base_uid += 1


class WorkflowList:
    """Alfred表对象, 用来存放WorkflowItem对象

    会将输出结果在Alfred中以列表形式来展现, 实际效果:
    |keyword query|
    |result1 ...  |
    |result2 ...  |
    ...
    |resultn ...  |

    >>> wf_list = WorkflowList()
    >>> wf_list.add_item(WorkflowItem('hello'))
    >>> wf_list.add_item(WorkflowItem('world'))
    >>> wf_list.output_to_alfred()
    {"items": [{"title": "hello", "arg": "hello", "valid": true, "uid": "1"}, {"title": "world", "arg": "world", "valid": true, "uid": "2"}]}
    """

    def __init__(self):
        self.items = []

    def __repr__(self):
        cls_name = type(self).__name__
        return '{}()'.format(cls_name)

    def add_item(self, wf_item: WorkflowItem) -> None:
        """向Alfred列表里追加一行

        wf_item: 一个WorkflowItem对象, 包含了一行的信息

        """
        self.items.append(wf_item)

    def output_to_alfred(self) -> None:
        """向标准输出stdout输出对象的json字符串格式"""
        json_repr = {}
        json_repr['items'] = [item.get_json_repr() for item in self.items]
        sys.stdout.write(json.dumps(json_repr))
