"""基础运动学变量定义
"""
import weakref

import math2d
from gameengine import uiobject
from gameengine import defines


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

    def __repr__(self):
        return '{}(velocity_acc={}, angular_acc={})'.format(self.__class__.__name__, self.velocity_acc, self.angular_acc)

    def __add__(self, other):
        return AccOutput(self.velocity_acc+other.velocity_acc, self.angular_acc+other.angular_acc)

    def __iadd__(self, other):
        sum = self + other
        self.velocity_acc = sum.velocity_acc
        self.angular_acc = sum.angular_acc
        return self

    def __mul__(self, other):
        return AccOutput(self.velocity_acc*other, self.angular_acc*other)

    def __rmul__(self, other):
        return self*other


class CollisionOutput(object):
    """碰撞输出"""
    def __init__(self, position: math2d.ndarray=math2d.position(), normal: math2d.ndarray=math2d.vector()):
        self.position = position  # 碰撞点坐标
        self.normal = normal  # 碰撞点法线


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
        self._body = uiobject.Circle(10)
        self._body.set_color(*defines.RED)
        self._head = uiobject.Triangle(6)
        self._head.set_color(*defines.GREEN)
        self._head.set_pos(12, 0)
        self._head.set_rotate(-90)
        self.add_child(self._body, 1)
        self.add_child(self._head)

    def update(self, dt):
        self.set_pos_vec(self.get_pos()+self.velocity*dt)
        self.set_rotate(math2d.as_degrees(self.velocity))


class DynamicObj(uiobject.UIObject, KinematicInterface):
    """动态运动物体

    以加速度驱动速度变化
    """
    def __init__(self, velocity, angular=0.0):
        super().__init__()
        self._radius = 10  # 半径
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
        self._body = uiobject.Circle(self._radius)
        self._body.set_color(*defines.RED)
        self._head = uiobject.Triangle(6)
        self._head.set_color(*defines.GREEN)
        self._head.set_pos(10, 0)
        self._head.set_rotate(-90)
        self.add_child(self._body, 1)
        self.add_child(self._head)

    def set_color(self, r: int, g: int, b: int):
        self._body.set_color(r, g, b)

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

    def radius(self) -> float:
        return self._radius

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


class LinePath(uiobject.UIObject):
    """直线路径
    """
    def __init__(self, points: list):
        super().__init__()
        self._lines = [math2d.Line(points[i], points[i+1]) for i in range(len(points)-1)]
        self._lines_obj = uiobject.Lines(points[1], *points[2:])
        self._lines_obj.set_pos_vec(points[0])
        self.add_child(self._lines_obj)

    def get_line_point(self, point: math2d.ndarray, offset: float):
        line_idx = self._get_near_line_idx(point)
        return self._get_point_by_offset(line_idx, point, offset)

    def _get_near_line_idx(self, point):
        near_idx, min_dis = -1, float('inf')
        for idx, line in enumerate(self._lines):
            near = line.near_point(point)
            dis = math2d.distance(point, near)
            if dis < min_dis:
                min_dis = dis
                near_idx = idx
        return near_idx

    def _get_point_by_offset(self, line_idx, point, offset):
        cur_point = self._lines[line_idx].near_point(point)
        while offset and line_idx < len(self._lines):
            cur_line = self._lines[line_idx]
            dis = math2d.distance(cur_line.end, cur_point)
            if dis <= offset:
                offset -= dis
                cur_point = cur_line.end
                line_idx += 1
            else:
                cur_point = cur_point+(math2d.normalize(cur_line.as_vector())*offset)
                offset = 0
        return cur_point


class CollisionDetector(object):
    """碰撞检测器"""
    def __init__(self, collision_lines):
        self._collision_lines = weakref.WeakSet(collision_lines)  # 碰撞线

    def get_collision(self, ray_line: math2d.Line):
        """检测碰撞

        未碰撞则返回None
        @param ray_line:
        @return:
        """
        nearest_line, nearest_distance = self._get_nearest(ray_line)
        if nearest_line:
            position = ray_line.begin + math2d.normalize(ray_line.as_vector())*nearest_distance
            normal = nearest_line.normal()
            if math2d.dot(ray_line.as_vector(), normal) >= 0:
                normal = -normal
            return CollisionOutput(position, normal)
        else:
            return None

    def _get_nearest(self, ray_line):
        nearest_distance = float('inf')
        nearest_line = None
        for line in self._collision_lines:
            if math2d.is_intersection(line, ray_line):
                point = math2d.line_intersect(line, ray_line)
                distance = math2d.distance(point, ray_line.begin)
                if distance < nearest_distance:
                    nearest_distance = distance
                    nearest_line = line
        return nearest_line, nearest_distance

