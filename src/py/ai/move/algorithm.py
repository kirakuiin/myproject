"""移动算法"""

import math2d


def get_acceleration(character, target, acc=100):
    """根据对象位置计算加速度
    """
    direction = math2d.normalize(target.get_world_pos() - character.get_world_pos())
    acceleration = direction * acc
    return acceleration


def get_acc_with_brake(src_pos, tar_pos, src_velocity, max_acc=100, max_speed=1000,
                       near_radius=20, brake_radius=100, near_time=0.1):
    """带有靠近减速的加速度计算
    """
    distance = math2d.distance(src_pos, tar_pos)
    if distance < near_radius:
        return math2d.vector(0, 0)
    if distance > brake_radius:
        speed = max_speed
    else:
        speed = max_speed * distance / brake_radius
    direction = math2d.normalize(tar_pos - src_pos)
    return get_match_speed(src_velocity, direction*speed, max_acc, near_time)


def get_rotate_with_bake(src_rot, tar_rot, src_angular, max_acc=100, max_angular=360,
                         near_radius=10, brake_radius=30, near_time=0.1):
    """带有靠近减速的角加速度计算"""
    rotation = math2d.angle_delta(src_rot, tar_rot)
    if abs(rotation) < near_radius:
        return 0
    if abs(rotation) > brake_radius:
        angular = max_angular
    else:
        angular = max_angular*rotation/brake_radius
    return get_match_speed(angular, src_angular, max_acc, near_time)


def get_match_speed(src_speed, des_speed, max_acc=100, near_time=0.1):
    """计算匹配指定角速度所需要的加速度

    @param src_angular: 源速度
    @param des_angular: 目标速度
    @param max_acc: 最大加速度
    @param near_time: 匹配时间
    @return: 加速度
    """
    acc = (des_speed-src_speed)/near_time
    if math2d.norm(acc) > max_acc:
        acc = math2d.normalize(acc) * max_acc
    return acc