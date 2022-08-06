"""引擎定时器

"""
import gameengine
from common import function

from . import gameobject


INFINITY = -1  # 无限重复


class Timer(gameobject.GameObject):
    """节点定时器

    """
    @classmethod
    def create(cls, cb_func, interval, loop_time=INFINITY, *args, **kwargs):
        """创建定时器

        自动附加到场景上
        @return:
        """
        instance = cls(cb_func, interval, loop_time, *args, **kwargs)
        gameengine.get_scene().add_child(instance)
        return instance

    def __init__(self, cb_func, interval, loop_time=INFINITY, *args, **kwargs):
        super().__init__()
        self._cb_func = function.Functor(cb_func, *args, **kwargs)
        self._interval = interval  # 调用间隔
        self._loop_time = loop_time  # 重复次数
        self._run_time = 0

    def update(self, dt: float):
        self._run_time += dt
        if self._loop_time == 0:
            self.stop()
        if self._run_time >= self._interval:
            self._loop_time -= 1
            self._run_time -= self._interval
            self._cb_func()

    def is_alive(self):
        return self._loop_time != 0

    def stop(self):
        parent = self.get_parent()
        parent and parent.remove_child(self)