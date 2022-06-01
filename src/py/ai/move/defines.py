"""基础运动学变量定义
"""

import math2d
from gameengine import uiobject


class KinematicInterface(object):
    """运动学接口"""
    def position(self) -> math2d.ndarray:
        """返回对象的位置

        @return:
        """

    def rotation(self) -> float:
        """返回对象的旋转度数

        @return:
        """

    def velocity(self) -> math2d.ndarray:
        """返回对象的速度

        @return:
        """

    def max_velocity(self) -> float:
        """返回最大速度

        @return:
        """

    def velocity_acc(self) -> math2d.ndarray:
        """返回对象的加速度

        @return:
        """

    def max_velocity_acc(self) -> float:
        """返回最大加速度

        @return:
        """

    def set_velocity_acc(self, acc: math2d.ndarray):
        """设置对象的加速度

        @param acc: 加速度
        @return:
        """

    def angular(self) -> float:
        """返回对象的角速度(度)

        @return:
        """

    def max_angular(self) -> float:
        """返回最大角速度

        @return:
        """

    def angular_acc(self) -> float:
        """返回对象的角加速度(度)

        @return:
        """

    def max_angular_acc(self) -> float:
        """返回最大角加速度

        @return:
        """

    def set_angular_acc(self, acc: float):
        """设置对象的角加速度

        @param acc: 角加速度
        @return:
        """

    def set_resistance_ratio(self, ratio):
        """设置阻力系数

        @param ratio: 阻力系数
        @return:
        """

    def set_constant(self, max_velocity: float=None, max_velocity_acc: float=None,
                      max_angular: float=None, max_angular_acc: float=None, resistance_ratio: float=None):
        """设置对象的运动常量

        如果不传参则保留现有值
        @param max_velocity: 最大速度
        @param max_velocity_acc: 最大加速度
        @param max_angular: 最大角速度
        @param max_angular_acc: 最大角加速度
        @param resistance_ratio: 阻力系数
        @return:
        """


class AccOutput(object):
    """加速度输出"""
    def __init__(self, velocity_acc=math2d.vector(), angular_acc=0):
        self.velocity_acc = velocity_acc  # 加速度
        self.angular_acc = angular_acc  # 角加速度


class StaticObj(uiobject.UIObject):
    """静态运动物体

    通过直接修改速度来驱动物体运动
    """
    def __init__(self, velocity):
        super().__init__()
        self.velocity = velocity
        self.set_rotate(math2d.as_degrees(velocity))
        self._init_ui()

    def _init_ui(self):
        body = uiobject.Circle(10)
        body.set_color(255, 0, 0)
        head = uiobject.Triangle(6)
        head.set_color(0, 255, 0)
        head.set_pos(12, 0)
        head.set_rotate(-90)
        self.add_child(body, 1)
        self.add_child(head)

    def update(self, dt):
        self.set_pos_vec(self.get_pos()+self.velocity*dt)
        self.set_rotate(math2d.as_degrees(self.velocity))


class DynamicObj(uiobject.UIObject, KinematicInterface):
    """动态运动物体

    以加速度驱动速度变化
    """
    def __init__(self, velocity, angular=0.0):
        super().__init__()
        self._velocity = velocity  # 速度
        self._angular = angular  # 角速度
        self._velocity_acc = math2d.vector(0, 0)  # 加速度
        self._angular_acc = 0  # 角加速度
        self._resistance_ratio = 10  # 阻力系数
        self._max_velocity_acc = 30  # 最大加速度
        self._max_angular_acc = 30  # 最大角加速度
        self._max_velocity = 100  # 最大速度
        self._max_angular = 100  # 最大角速度

        self._init_ui()

    def _init_ui(self):
        body = uiobject.Circle(10)
        body.set_color(255, 0, 0)
        head = uiobject.Triangle(6)
        head.set_color(0, 255, 0)
        head.set_pos(10, 0)
        head.set_rotate(-90)
        self.add_child(body, 1)
        self.add_child(head)

    def update(self, dt):
        half_t_sq = 0.5*(dt**2)
        acc = self._velocity_acc - self._resistance_ratio * math2d.normalize(self._velocity)
        self.set_pos_vec(self.get_pos() + self._velocity*dt + half_t_sq*acc)
        self.set_rotate(self.get_rotate() + self._angular*dt + half_t_sq*self._angular_acc)
        self._velocity = self._velocity + acc*dt
        self._angular = self._angular + self._angular_acc*dt

        if math2d.norm(self._velocity) > self._max_velocity:
            self._velocity = math2d.normalize(self._velocity) * self._max_velocity
        if math2d.norm(self._angular) > self._max_angular:
            self._angular = math2d.normalize(self._angular) * self._max_angular

    def position(self) -> math2d.ndarray:
        return self.get_world_pos()

    def rotation(self) -> float:
        return self.get_world_rotate()

    def velocity(self) -> math2d.ndarray:
        return self._velocity

    def velocity_acc(self) -> math2d.ndarray:
        return self._velocity_acc

    def max_velocity(self) -> float:
        return self._max_velocity

    def max_velocity_acc(self) -> float:
        return self._max_velocity_acc

    def angular(self) -> float:
        return self._angular

    def angular_acc(self) -> float:
        return self._angular_acc

    def max_angular(self) -> float:
        return self._max_angular

    def max_angular_acc(self) -> float:
        return self._max_angular_acc

    def set_velocity_acc(self, acc: math2d.ndarray):
        self._velocity_acc = acc

    def set_angular_acc(self, acc: float):
        self._angular_acc = acc

    def set_resistance_ratio(self, ratio):
        self._resistance_ratio = ratio

    def set_constant(self, max_velocity: float=None, max_velocity_acc: float=None,
                      max_angular: float=None, max_angular_acc: float=None, resistance_ratio: float=None):
        self._set_value_if_not_none('_max_velocity', max_velocity)
        self._set_value_if_not_none('_max_velocity_acc', max_velocity_acc)
        self._set_value_if_not_none('_max_angular', max_angular)
        self._set_value_if_not_none('_max_angular_acc', max_angular_acc)
        self._set_value_if_not_none('_resistance_ratio', resistance_ratio)

    def _set_value_if_not_none(self, attr, value):
        if value is not None:
            setattr(self, attr, value)