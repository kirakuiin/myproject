"""ai实例入口
"""

import sys
import argparse


def init_syspath():
    sys.path.append('C:/Users/admin/myproject/src/py/ai')


init_syspath()


import gameengine
import run_case


class ArgParser(object):
    """参数解析器
    """
    def __init__(self):
        self._init_parser()

    def _init_parser(self):
        self._parser = argparse.ArgumentParser(description='ai示例演示工具')  # 参数解析器
        self._parser.add_argument('-t', '--type', type=str, required=False,
                                  help='运行某个类型的全部示例')
        self._parser.add_argument('-i', '--index', type=int, required=False,
                                  default=0,
                                  help='从某个类型的第几个开始执行')
        self._parser.add_argument('-n', '--name', type=str, required=False,
                                  help='执行某个类型指定的类')
        self._parser.add_argument('-s', dest='single', action='store_true', help='仅运行单个示例')
        self._parser.add_argument('-k', dest='track', action='store_true', help='显示绘制轨迹')
        self._parser.add_argument('-d', dest='display', action='store_true', help='显示全部示例名称')

        self._args = self._parser.parse_args()

    def get_type(self) -> str:
        return self._args.type

    def get_index(self) -> int:
        return self._args.index

    def get_cls_name(self) -> str:
        return self._args.name

    def is_single(self) -> bool:
        return self._args.single

    def is_show_track(self) -> bool:
        return self._args.track

    def is_display_info(self) -> bool:
        return self._args.display


def _run_case_by_param(arg_parser):
    base_type, idx, name = arg_parser.get_type(), arg_parser.get_index(), arg_parser.get_cls_name()
    if not base_type:
        run_case.run_all_case()
        return
    if arg_parser.is_single() and name:
        run_case.run_single_by_name(base_type, name)
        return
    if arg_parser.is_single() and not name:
        run_case.run_single_by_idx(base_type, idx)
        return
    run_case.run_by_type(base_type, idx)


if __name__ == '__main__':
    parser = ArgParser()
    if parser.is_display_info():
        print(run_case.get_all_case_info())
    else:
        gameengine.set_show_track(parser.is_show_track())
        _run_case_by_param(parser)

