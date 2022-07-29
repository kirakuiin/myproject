"""函数
"""

import weakref
import types


class Functor(object):
    """弱引用函数包装"""
    @classmethod
    def get_weak_func(cls, func) -> weakref.ReferenceType:
        """返回弱引用函数对象"""
        if isinstance(func, types.MethodType):
            return weakref.WeakMethod(func)
        else:
            return weakref.ref(func)

    def __init__(self, func, *args, **kwargs):
        self._func = self.get_weak_func(func)
        self._args = args
        self._kwargs = kwargs
        self._is_call_param_first = False  # 是否传入参数优先

    def set_is_call_param_first(self, is_first: bool):
        """设置是否传入参数优先

        @param is_first:
        @return:
        """
        self._is_call_param_first = is_first

    def __call__(self, *args, **kwargs):
        cur_kwargs = dict(self._kwargs)
        cur_kwargs.update(kwargs)
        cur_args = args+self._args if self._is_call_param_first else self._args+args
        return self._func()(*cur_args, **cur_kwargs)


def finalize(func, destroy, *args, **kwargs):
    """返回对象销毁器
    """
    if isinstance(func, types.MethodType):
        return weakref.finalize(func.__self__, destroy, *args, **kwargs)
    else:
        return weakref.finalize(func, destroy, *args, **kwargs)