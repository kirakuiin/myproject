"""移动算法"""

import math2d
from .defines import KinematicInterface as Kinematic
from .defines import AccOutput as Output


def get_seek_acc(src_pos, des_pos, acc):
    """根据对象位置寻找加速度
    """
    direction = math2d.normalize(des_pos - src_pos)
    acceleration = direction * acc
    return acceleration


def get_arrive_acc(src_obj: Kinematic, des_obj: Kinematic, brake_radius=100, near_time=0.25):
    """计算到达加速度
    """
    speed = get_speed_by_distance(math2d.distance(src_obj.position(), des_obj.position()), src_obj.max_velocity(),
                                  brake_radius)
    direction = math2d.normalize(des_obj.position() - src_obj.position())
    return get_match_acc(src_obj.velocity(), direction * speed, src_obj.max_velocity_acc(), near_time)


def get_align_acc(src_obj: Kinematic, des_rot: float, brake_radius=30, near_time=0.25):
    """计算对齐角加速度"""
    angular = get_speed_by_distance(abs(math2d.angle_delta(src_obj.rotation(), des_rot)), src_obj.max_angular(),
                                    brake_radius)
    direction = math2d.normalize(math2d.angle_delta(src_obj.rotation(), des_rot))
    return get_match_acc(src_obj.angular(), angular * direction, src_obj.max_angular_acc(), near_time)


def get_chase_acc(src_obj: Kinematic, des_obj: Kinematic, max_predict_time=0.25):
    """计算追逐加速度

    通过预测目标的位置来设置方向
    """
    distance = math2d.distance(src_obj.position(), des_obj.position())
    predict_time = distance / math2d.norm(src_obj.velocity())
    predict_time = min(max_predict_time, predict_time)
    future_des_pos = des_obj.position() + predict_time * des_obj.velocity()
    return get_seek_acc(src_obj.position(), future_des_pos, src_obj.max_velocity_acc())


def get_face_acc(src_obj: Kinematic, des_pos: math2d.ndarray, brake_radius=30, near_time=0.25):
    """计算朝向角加速度
    """
    des_rot = math2d.as_degrees(des_pos - src_obj.position())
    return get_align_acc(src_obj, des_rot, brake_radius, near_time)


def get_look_direct_acc(src_obj: Kinematic, brake_radius=30, near_time=0.25):
    """计算直视速度方向角加速度
    """
    des_rot = math2d.as_degrees(src_obj.velocity())
    return get_align_acc(src_obj, des_rot, brake_radius, near_time)


def get_wander_acc(src_obj: Kinematic, brake_radius=30, near_time=0.25,
                   wander_offset=100, wander_radius=100, wander_rate=80) -> Output:
    """计算漫游加速度

    在对象的前方创建一个虚拟目标, 虚拟目标出现在以对象前方固定偏移为圆心的圆环的某个位置上
    """
    circle_center = src_obj.position() + math2d.as_vector(src_obj.rotation())*wander_offset
    target_pos = circle_center + math2d.as_vector(src_obj.rotation()+math2d.rand_bio()*wander_rate)*wander_radius
    angular_acc = get_face_acc(src_obj, target_pos, brake_radius, near_time)
    velocity_acc = src_obj.max_velocity_acc()*math2d.as_vector(src_obj.rotation())
    return Output(velocity_acc, angular_acc)


def get_speed_by_distance(distance, max_speed, brake_radius):
    """根据距离来计算合适的速度

    距离越近速度越低, 当距离为0时速度应为0
    @param distance: 距离
    @param max_speed: 最大速度
    @param brake_radius: 减速半径
    @return:
    """
    if distance > brake_radius:
        return max_speed
    else:
        return max_speed * distance / brake_radius


def get_match_acc(src_speed, des_speed, max_acc=100.0, near_time=0.25):
    """计算匹配指定角速度所需要的加速度

    @param src_speed: 源速度
    @param des_speed: 目标速度
    @param max_acc: 最大加速度
    @param near_time: 匹配时间
    @return: 加速度
    """
    acc = (des_speed - src_speed) / near_time
    if math2d.norm(acc) > max_acc:
        acc = math2d.normalize(acc) * max_acc
    return acc


def is_close_enough(src_pos: math2d.ndarray, des_pos: math2d.ndarray, near_threshold=5):
    """判断两个对象是否足够接近

    @param src_pos: 来源位置
    @param des_pos: 目标位置
    @param near_threshold: 接近阈值
    @return:
    """
    return math2d.distance(src_pos, des_pos) < near_threshold
