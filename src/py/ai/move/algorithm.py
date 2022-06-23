"""移动算法"""

import math2d
from .defines import KinematicInterface as Kinematic


def get_seek_acc(src_pos: math2d.ndarray, des_pos: math2d.ndarray, acc: float) -> math2d.ndarray:
    """根据对象位置寻找加速度

    @param src_pos: 源位置
    @param des_pos: 目标位置
    @param acc: 加速度
    """
    direction = math2d.normalize(des_pos - src_pos)
    acceleration = direction * acc
    return acceleration


def get_align_acc(src_obj: Kinematic, des_rot: float, brake_radius=30, near_time=0.25):
    """计算对齐角加速度

    @param src_obj: 源对象
    @param des_rot: 目标角度
    @param brake_radius: 减速半径
    @param near_time: 接近时间
    @return:
    """
    angular = get_speed_by_distance(abs(math2d.angle_delta(src_obj.rotation(), des_rot)), src_obj.max_angular(),
                                    brake_radius)
    direction = math2d.normalize(math2d.angle_delta(src_obj.rotation(), des_rot))
    return get_match_acc(src_obj.angular(), angular * direction, src_obj.max_angular_acc(), near_time)


def get_face_acc(src_obj: Kinematic, des_pos: math2d.ndarray, brake_radius=30, near_time=0.25):
    """计算朝向角加速度

    @param src_obj: 源对象
    @param des_pos: 目标位置
    @param brake_radius: 减速半径
    @param near_time: 接近时间
    @return:
    """
    des_rot = math2d.as_degrees(des_pos - src_obj.position())
    return get_align_acc(src_obj, des_rot, brake_radius, near_time)


def get_speed_by_distance(distance: float, max_speed: float, brake_radius: float):
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


def get_match_acc(src_speed, des_speed, max_acc=100.0, near_time=0.25) -> math2d.ndarray:
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


def is_close_enough(src_pos: math2d.ndarray, des_pos: math2d.ndarray, near_threshold=5) -> bool:
    """判断两个对象是否足够接近

    @param src_pos: 来源位置
    @param des_pos: 目标位置
    @param near_threshold: 接近阈值
    @return:
    """
    return math2d.distance(src_pos, des_pos) < near_threshold


def get_near_time(src_pos: math2d.ndarray, src_vec: math2d.ndarray, des_pos: math2d.ndarray,
                  des_vec: math2d.ndarray) -> float:
    """判断两个对象到达最近距离所需要的时间

    如果返回值小于0则代表永远不会接近
    @param src_pos: 来源位置
    @param src_vec: 来源速度
    @param des_pos: 目标位置
    @param des_vec: 目标速度
    @return: float
    """
    dp = des_pos - src_pos
    dv = src_vec - des_vec
    length = math2d.norm(dv)
    if length <= 0:
        return 0 if math2d.norm(dp) == 0 else -1
    return math2d.dot(dp, dv) / length ** 2