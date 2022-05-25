"""角度相关"""

import math


class AngleType(object):
    """角度类型"""
    RADIANS = 1  # 弧度
    DEGREES = 2  # 度数


def angle_delta(src_angle: float, target_angle: float, unit: AngleType=AngleType.DEGREES):
    """角度差值

    差值会在[-pi, pi]之间
    @param src_angle:
    @param target_angle:
    @param unit: 角度制还是弧度制
    @return:
    """
    if unit == AngleType.RADIANS:
        target_angle, src_angle = math.degrees(target_angle), math.degrees(src_angle)
    pi = int(math.degrees(math.pi))
    delta = (target_angle - src_angle) % (2*pi)
    delta = delta if delta <= pi else delta-2*pi
    return delta if unit == AngleType.DEGREES else math.radians(delta)


