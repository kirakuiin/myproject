"""信号
"""

from . import function


class Signal(object):
    """信号
    """
    def __init__(self):
        self._cb_dict = {}

    def __repr__(self):
        return '{}(cb={})'.format(self.__class__.__name__, self._cb_dict)

    def connect(self, callback):
        if self._find_id_in_list(callback) is None:
            cb_id = id(callback)
            self._cb_dict[cb_id] = function.Functor(callback)
            function.finalize(callback, self._destroy, cb_id)

    def _find_id_in_list(self, callback):
        cb_id = id(callback)
        if cb_id in self._cb_dict:
            return cb_id
        else:
            return None

    def _destroy(self, cb_id):
        self._cb_dict.pop(cb_id, None)

    def disconnect(self, callback):
        cb_id = self._find_id_in_list(callback)
        self._cb_dict.pop(cb_id, None)

    def emit(self, *args, **kwargs):
        for cb in self._cb_dict.values():
            cb(*args, **kwargs)