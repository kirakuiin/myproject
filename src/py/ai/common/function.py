"""函数
"""

import weakref
import types


def weak_func(func) -> weakref.ReferenceType:
    """返回弱引用函数对象"""
    if isinstance(func, types.MethodType):
        return weakref.WeakMethod(func)
    else:
        return weakref.ref(func)


def finalize(func, destroy, *args, **kwargs):
    """返回对象销毁器
    """
    if isinstance(func, types.MethodType):
        return weakref.finalize(func.__self__, destroy, *args, **kwargs)
    else:
        return weakref.finalize(func, destroy, *args, **kwargs)