"""转向行为
"""

import math2d
import collections
import weakref

from . import defines
from . import algorithm


class SteeringBehaviorInterface(object):
    """转向行为接口"""
    def get_steering_output(self) -> defines.AccOutput:
        """返回转向输出

        @return:
        """
        pass


class BlendedSteering(SteeringBehaviorInterface):
    """转向行为混合"""
    Pair = collections.namedtuple('Pair', 'behavior, weight')

    def __init__(self, src_obj: defines.KinematicInterface):
        self._pairs = []  # 行为对列表
        self._src_obj = weakref.proxy(src_obj)  # 源目标

    def add_behavior(self, behavior: SteeringBehaviorInterface, weight: float=1.0):
        """混合行为

        @param behavior: 行为
        @param weight: 权重
        @return:
        """
        pair = BlendedSteering.Pair(behavior, weight)
        self._pairs.append(pair)

    def get_steering_output(self) -> defines.AccOutput:
        output = defines.AccOutput()
        for pair in self._pairs:
            output += pair.weight*pair.behavior.get_steering_output()
        output.velocity_acc = min(self._src_obj.max_velocity_acc(), math2d.norm(output.velocity_acc)) * math2d.normalize(output.velocity_acc)
        output.angular_acc = min(self._src_obj.max_angular_acc(), math2d.norm(output.angular_acc)) * math2d.normalize(output.angular_acc)
        return output


class PrioritySteering(SteeringBehaviorInterface):
    """转向行为按优先级混合"""
    EPSILON = 1e-6

    def __init__(self):
        self._priority_behaviors = []  # 行为组

    def add_behavior(self, group: SteeringBehaviorInterface):
        """添加行为组

        先添加的优先级高
        @param group: 行为组
        @return:
        """
        self._priority_behaviors.append(group)

    def get_steering_output(self) -> defines.AccOutput:
        output = defines.AccOutput()
        for group in self._priority_behaviors:
            output = group.get_steering_output()
            if math2d.norm(output.angular_acc) > self.EPSILON or math2d.norm(output.velocity_acc) > self.EPSILON:
                return output
        return output


class SeekBehavior(SteeringBehaviorInterface):
    """寻找行为"""
    def __init__(self, src_obj: defines.KinematicInterface, des_obj: defines.KinematicInterface):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)

    def get_steering_output(self) -> defines.AccOutput:
        acc = algorithm.get_seek_acc(self._src_obj.position(), self._des_obj.position(), self._src_obj.max_velocity_acc())
        return defines.AccOutput(acc)


class ArriveBehavior(SteeringBehaviorInterface):
    """到达行为"""
    def __init__(self, src_obj, des_obj, brake_radius=100, near_time=0.25):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)
        self._brake_radius = brake_radius  # 减速半径
        self._near_time = near_time  # 接近时间

    def get_steering_output(self) -> defines.AccOutput:
        distance = math2d.distance(self._src_obj.position(), self._des_obj.position())
        speed = algorithm.get_speed_by_distance(distance, self._src_obj.max_velocity(), self._brake_radius)
        direction = math2d.normalize(self._des_obj.position() - self._src_obj.position())
        return defines.AccOutput(algorithm.get_match_acc(self._src_obj.velocity(), direction * speed,
                                                         self._src_obj.max_velocity_acc(), self._near_time))


class AlignBehavior(SteeringBehaviorInterface):
    """对齐行为"""
    def __init__(self, src_obj, des_obj, brake_radius=30, near_time=0.25):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)
        self._brake_radius = brake_radius  # 减速半径
        self._near_time = near_time  # 接近时间

    def get_steering_output(self) -> defines.AccOutput:
        acc = algorithm.get_align_acc(self._src_obj, self._des_obj.rotation(), self._brake_radius, self._near_time)
        return defines.AccOutput(angular_acc=acc)


class VelocityMatchBehavior(SteeringBehaviorInterface):
    """速度匹配行为"""
    def __init__(self, src_obj, des_obj, near_time=0.25):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)
        self._near_time = near_time  # 接近时间

    def get_steering_output(self) -> defines.AccOutput:
        acc = algorithm.get_match_acc(self._src_obj.velocity(), self._des_obj.velocity(),
                                      self._src_obj.max_velocity_acc(), self._near_time)
        return defines.AccOutput(acc)


class ChaseBehavior(SteeringBehaviorInterface):
    """追逐行为"""
    def __init__(self, src_obj, des_obj, max_predict_time=1):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)
        self._max_predict_time = max_predict_time  # 最大预测时间

    def get_steering_output(self) -> defines.AccOutput:
        distance = math2d.distance(self._src_obj.position(), self._des_obj.position())
        predict_time = distance / math2d.norm(self._src_obj.velocity())
        predict_time = min(self._max_predict_time, predict_time)
        future_des_pos = self._des_obj.position() + predict_time * self._des_obj.velocity()
        return defines.AccOutput(algorithm.get_seek_acc(
            self._src_obj.position(), future_des_pos, self._src_obj.max_velocity_acc()))


class FaceBehavior(SteeringBehaviorInterface):
    """朝向行为"""
    def __init__(self, src_obj, des_obj, brake_radius=30, near_time=0.25):
        self._src_obj = weakref.proxy(src_obj)
        self._des_obj = weakref.proxy(des_obj)
        self._brake_radius = brake_radius  # 减速半径
        self._near_time = near_time  # 接近时间

    def get_steering_output(self) -> defines.AccOutput:
        acc = algorithm.get_face_acc(self._src_obj, self._des_obj.position(), self._brake_radius, self._near_time)
        return defines.AccOutput(angular_acc=acc)


class LookDirectBehavior(SteeringBehaviorInterface):
    """直视速度方向行为"""
    def __init__(self, src_obj, brake_radius=30, near_time=0.25):
        self._src_obj = weakref.proxy(src_obj)
        self._brake_radius = brake_radius  # 减速半径
        self._near_time = near_time  # 接近时间

    def get_steering_output(self) -> defines.AccOutput:
        des_rot = math2d.as_degrees(self._src_obj.velocity())
        return defines.AccOutput(angular_acc=algorithm.get_align_acc(
            self._src_obj, des_rot, self._brake_radius, self._near_time))


class WanderBehavior(SteeringBehaviorInterface):
    """漫游行为"""
    def __init__(self, src_obj, brake_radius=30, near_time=0.25, wander_offset=100, wander_radius=100, wander_degree=80):
        self._src_obj = weakref.proxy(src_obj)
        self._brake_radius = brake_radius  # 减速半径
        self._near_time = near_time  # 接近时间
        self._wander_offset = wander_offset  # 漫游偏移
        self._wander_radius = wander_radius  # 漫游半径
        self._wander_degree = wander_degree  # 漫游角度

    def get_steering_output(self) -> defines.AccOutput:
        # 创建虚拟圆环, 并随机其上一点
        circle_center = self._src_obj.position() + math2d.as_vector(self._src_obj.rotation()) * self._wander_offset
        target_pos = circle_center + math2d.as_vector(self._src_obj.rotation() + math2d.rand_bio() * self._wander_degree) * self._wander_radius

        angular_acc = algorithm.get_face_acc(self._src_obj, target_pos, self._brake_radius, self._near_time)
        velocity_acc = self._src_obj.max_velocity_acc() * math2d.as_vector(self._src_obj.rotation())
        return defines.AccOutput(velocity_acc, angular_acc)


class PathFollowingBehavior(SteeringBehaviorInterface):
    """路径跟随行为"""
    def __init__(self, src_obj, line_path: defines.LinePath, offset=100):
        self._src_obj = weakref.proxy(src_obj)
        self._line_path = weakref.proxy(line_path)  # 路径对象
        self._offset = offset  # 寻路偏移

    def get_steering_output(self) -> defines.AccOutput:
        near_point = self.get_near_point()
        return defines.AccOutput(algorithm.get_seek_acc(
            self._src_obj.position(), near_point, self._src_obj.max_velocity_acc()))

    def get_near_point(self):
        return self._line_path.get_line_point(self._src_obj.position(), self._offset)


class SeparationBehavior(SteeringBehaviorInterface):
    """分离行为"""
    def __init__(self, src_obj, des_objs, threshold=100.0, decay_coe=100.0):
        self._src_obj = weakref.proxy(src_obj)
        self._des_objs = weakref.WeakSet(des_objs)  # 分离对象列表
        self._threshold = threshold  # 触发分离操作阈值
        self._decay_coe = decay_coe # 斥力衰变系数

    def get_steering_output(self) -> defines.AccOutput:
        final_acc = math2d.vector()
        for des in self._des_objs:
            distance = math2d.distance(self._src_obj.position(), des.position())
            if distance < self._threshold:
                direction = math2d.normalize(self._src_obj.position() - des.position())
                final_acc += self._decay_coe * (self._threshold-distance) / self._threshold * direction
        final_acc = math2d.normalize(final_acc) * min(self._src_obj.max_velocity_acc(), math2d.norm(final_acc))
        return defines.AccOutput(final_acc)


class CollisionAvoidanceBehavior(SteeringBehaviorInterface):
    """避免碰撞行为"""
    def __init__(self, src_obj, des_objs):
        self._src_obj = weakref.proxy(src_obj)
        self._des_objs = weakref.WeakSet(des_objs)  # 分离对象列表

    def get_steering_output(self) -> defines.AccOutput:
        des_obj, shortest_time = self._find_nearest_obj()
        if des_obj is None:
            return defines.AccOutput()
        src_future_pos = self._src_obj.position() + shortest_time*self._src_obj.velocity()
        des_future_pos = des_obj.position() + shortest_time*des_obj.velocity()
        nearest_dis, cur_dis = (math2d.distance(src_future_pos, des_future_pos), math2d.distance(self._src_obj.position(), des_obj.position()))
        if nearest_dis <= 0 or cur_dis <= 2*self._src_obj.radius():
            direction = self._src_obj.position() - des_obj.position()
        else:
            direction = src_future_pos - des_future_pos
        return defines.AccOutput(self._src_obj.max_velocity_acc() * math2d.normalize(direction))

    def _find_nearest_obj(self):
        shortest_time = float('inf')
        first_collision_obj = None
        for des in self._des_objs:
            near_time = algorithm.get_near_time(self._src_obj.position(), self._src_obj.velocity(), des.position(), des.velocity())
            distance = math2d.distance(self._src_obj.position() + near_time * self._src_obj.velocity(),
                                       des.position() + near_time * des.velocity())
            if 0 <= near_time < shortest_time and distance <= 2 * self._src_obj.radius():
                first_collision_obj = des
                shortest_time = near_time
        return first_collision_obj, shortest_time


class ObstacleAvoidanceBehavior(SteeringBehaviorInterface):
    """避障行为"""
    def __init__(self, src_obj, detector: defines.CollisionDetector, look_ahead=100, avoid_dis=50):
        self._src_obj = weakref.proxy(src_obj)
        self._detector = weakref.proxy(detector)  # 障碍检测器
        self._look_ahead = look_ahead  # 探测射线长度
        self._avoid_dis = avoid_dis  # 法线反射距离

    def get_steering_output(self) -> defines.AccOutput:
        begin = self._src_obj.position()
        end = begin+math2d.normalize(self._src_obj.velocity())*self._look_ahead
        collision = self._detector.get_collision(math2d.Line(begin, end))
        if collision:
            return defines.AccOutput(algorithm.get_seek_acc(
                self._src_obj.position(), collision.position+collision.normal*self._avoid_dis, self._src_obj.max_velocity_acc()))
        else:
            return defines.AccOutput()
