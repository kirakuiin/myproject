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
                                  default=0, help='从某个类型的第几个开始执行')
        self._parser.add_argument('-n', '--name', type=str, required=False,
                                  help='执行某个类型指定的类')
        self._parser.add_argument('-m', '--times', type=float, required=False,
                                  help='实例运行速度倍数', default=1.0)
        self._parser.add_argument('-c', '--coord', type=int, required=False,
                                  help='显示坐标轴(0为不显示, 大于0则设置单位)', default=200)
        self._parser.add_argument('-s', dest='single', action='store_true', help='仅运行单个示例')
        self._parser.add_argument('-d', dest='display', action='store_true', help='显示全部示例名称')

        self._args = self._parser.parse_args()

    def get_type(self) -> str:
        return self._args.type

    def get_index(self) -> int:
        return self._args.index

    def get_cls_name(self) -> str:
        return self._args.name

    def get_times(self) -> float:
        return self._args.times

    def get_coord_unit(self) -> int:
        return abs(self._args.coord)

    def is_single(self) -> bool:
        return self._args.single

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


def _show_run_config(arg_parser):
    print('=====配置清单=====')
    print('是否运行单个示例: {}'.format(arg_parser.is_single()))
    print('运行速度: {:.1f}x'.format(arg_parser.get_times()))
    coord = arg_parser.get_coord_unit()
    if coord > 0:
        print('坐标轴单位: {} pix'.format(coord))
    else:
        print('坐标轴: 关闭')
    print('=====示例输出=====')


if __name__ == '__main__':
    parser = ArgParser()
    if parser.is_display_info():
        print(run_case.get_all_case_info())
    else:
        _show_run_config(parser)
        gameengine.set_speed(parser.get_times())
        run_case.set_case_param(coord_unit=parser.get_coord_unit())
        _run_case_by_param(parser)

