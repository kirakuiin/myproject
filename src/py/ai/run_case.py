"""运行示例代码

示例的存储结构如下所示:
{
'type_a': [case1, case2, case3, ...],
'type_b': [case1, case2, case3, ...],
...
}

"""

# 导入测试模块
import test
import move
import path
import decision
# 导入测试模块结束

import case
import pprint


case_param = {}  # 用例执行参数


def set_case_param(**kwargs):
    """设置用例参数
    @param kwargs:
    @return:
    """
    global case_param
    case_param = kwargs


def get_all_case_info() -> str:
    """返回全部示例信息

    @return:
    """
    info = ['########## 示例信息 ##########']
    for type, case_list in case.get_case_container().items():
        list_str = ['[{}]{}({})'.format(idx, elem.__name__, elem.__doc__) for idx, elem in enumerate(case_list)]
        info.append('类型: {}\n列表: \n{}\n'.format(type, pprint.pformat(list_str, 1)))
    info.append('\n')
    return '\n'.join(info)


def run_all_case():
    """运行全部示例

    @return:
    """
    print('运行全部示例')
    for base_type in case.get_all_type():
        for case_cls in case.get_case_by_type(base_type):
            _run_instance(case_cls)
    print('运行结束')


def run_by_type(base_type: str, start=0):
    """运行指定下标开始运行指定类型的示例

    @param base_type: 类型
    @param start: 起始下标
    @return:
    """
    print('运行从下标{}开始, 类别为[{}]的所有示例'.format(start, base_type))
    for case_cls in case.get_case_by_type(base_type)[start:]:
        _run_instance(case_cls)
    print('运行结束')


def run_single_by_idx(base_type: str, idx: int=0):
    """根据类型和下标来运行单个示例

    @param base_type:
    @param idx:
    @return:
    """
    for case_cls in case.get_case_by_type(base_type)[idx:idx + 1]:
        _run_instance(case_cls)
        break
    else:
        print('下标越界 {}>{}'.format(idx, len(case.get_case_by_type(base_type))-1))


def run_single_by_name(base_type: str, cls_name: str):
    """根据类型和名称运行单个示例

    @param base_type:
    @param cls_name:
    @return:
    """
    for case_cls in case.get_case_by_type(base_type):
        if case_cls.__name__ == cls_name:
            _run_instance(case_cls)
            break
    else:
        print('无名为[{}]的示例'.format(cls_name))


def _run_instance(cls):
    instance = cls()
    instance.set_case_param(**case_param)
    instance.init_engine()
    instance.init_case()
    instance.start_engine()
