"""基础运动学常量定义
"""

import math2d
from gameengine import uiobject


class StaticObj(uiobject.UIObject):
    """静态运动物体

    通过直接修改速度来驱动物体运动
    """
    def __init__(self, velocity):
        super().__init__()
        self.velocity = velocity
        self.set_rotate(math2d.vector_degrees(velocity))
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
        self.set_rotate(math2d.vector_degrees(self.velocity))


class KinematicObj(uiobject.UIObject):
    """动态运动物体

    以加速度驱动速度变化
    """
    def __init__(self, velocity, angular=0.0):
        super().__init__()
        self.velocity = velocity  # 速度
        self.angular = angular  # 角速度
        self.acceleration = math2d.vector(0, 0)  # 加速度
        self.angular_acc = 0  # 角加速度
        self.resist_ratio = 100  # 阻力系数
        self.max_speed = float('inf')  # 最大速度
        self.max_angular = float('inf')  # 最大角速度

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
        half_t_sq = 0.5*(dt**2)
        acc = self.acceleration - self.resist_ratio*math2d.normalize(self.velocity)
        self.set_pos_vec(self.get_pos() + self.velocity*dt + half_t_sq*acc)
        self.set_rotate(self.get_rotate() + self.angular*dt + half_t_sq*self.angular_acc)
        self.velocity = self.velocity + acc*dt
        self.angular = self.angular + self.angular_acc*dt

        if math2d.norm(self.velocity) > self.max_speed:
            self.velocity = math2d.normalize(self.velocity) * self.max_speed
        self.angular = min(self.max_angular, self.angular)