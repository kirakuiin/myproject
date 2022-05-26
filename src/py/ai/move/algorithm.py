"""移动算法"""

import math2d


def get_seek_acc(src_pos, des_pos, acc=100):
    """根据对象位置寻找加速度
    """
    direction = math2d.normalize(des_pos - src_pos)
    acceleration = direction * acc
    return acceleration


def get_arrive_acc(src_pos, des_pos, src_velocity, max_acc=100, max_speed=1000,
                       brake_radius=100, near_time=0.25):
    """计算到达加速度
    """
    speed = get_speed_by_distance(math2d.distance(src_pos, des_pos), max_speed, brake_radius)
    direction = math2d.normalize(des_pos - src_pos)
    return get_match_acc(src_velocity, direction * speed, max_acc, near_time)


def get_align_acc(src_rot, des_rot, src_angular, max_acc=100, max_angular=360,
                         brake_radius=30, near_time=0.25):
    """计算对齐角加速度"""
    angular = get_speed_by_distance(abs(math2d.angle_delta(src_rot, des_rot)), max_angular, brake_radius)
    direction = math2d.normalize(math2d.angle_delta(src_rot, des_rot))
    return get_match_acc(src_angular, angular * direction, max_acc, near_time)


def get_chase_acc(src_pos, des_pos, src_velocity, des_velocity, max_predict_time=0.25, acc=100):
    """计算追逐加速度

    通过预测目标的位置来设置方向
    """
    distance = math2d.distance(src_pos, des_pos)
    predict_time = distance / math2d.norm(src_velocity)
    predict_time = min(max_predict_time, predict_time)
    future_des_pos = des_pos + predict_time*des_velocity
    return get_seek_acc(src_pos, future_des_pos, acc)


def get_face_acc(src_pos, src_rot, des_pos, src_angular, max_acc=100, max_angular=360,
                 brake_radius=30, near_time=0.25):
    """计算朝向角加速度
    """
    des_rot = math2d.vector_degrees(des_pos-src_pos)
    return get_align_acc(src_rot, des_rot, src_angular, max_acc, max_angular,
                         brake_radius, near_time)


def get_look_direct_acc(src_rot, src_velocity, src_angular, max_acc=100, max_angular=360,
                        brake_radius=30, near_time=0.25):
    """计算直视速度方向角加速度
    """
    des_rot = math2d.vector_degrees(src_velocity)
    return get_align_acc(src_rot, des_rot, src_angular, max_acc, max_angular,
                         brake_radius, near_time)


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
        return max_speed * distance/brake_radius


def get_match_acc(src_speed, des_speed, max_acc=100, near_time=0.25):
    """计算匹配指定角速度所需要的加速度

    @param src_speed: 源速度
    @param des_speed: 目标速度
    @param max_acc: 最大加速度
    @param near_time: 匹配时间
    @return: 加速度
    """
    acc = (des_speed-src_speed)/near_time
    if math2d.norm(acc) > max_acc:
        acc = math2d.normalize(acc) * max_acc
    return acc


def is_close_enough(src_obj, des_obj, near_threshold=5):
    """判断两个对象是否足够接近

    @param src_obj: 来源
    @param des_obj: 目标
    @param near_threshold: 接近阈值
    @return:
    """
    return math2d.distance(src_obj.get_world_pos(), des_obj.get_world_pos()) < near_threshold