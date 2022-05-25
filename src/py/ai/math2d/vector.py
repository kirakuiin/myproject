"""向量运算"""

import functools
from numpy import *


def vector(x: float=0.0, y: float=0.0) -> ndarray:
    """向量

    @param x: x分量
    @param y: y分量
    @return:
    """
    return array([x, y, 0])


def position(x: float=0, y: float=0) -> ndarray:
    """位置

    @param x: x坐标
    @param y: y坐标
    @return:
    """
    return array([x, y, 1])


@functools.singledispatch
def norm(value):
    """模

    @param value: 被取模的值
    @return:
    """
    return abs(value)


@norm.register
def _(vec: ndarray) -> float:
    return linalg.norm(vec)


@functools.singledispatch
def normalize(value):
    """归一化

    @param value: 被归一化单位
    @return:
    """
    return value/abs(value)


@normalize.register
def _(vec: ndarray) -> ndarray:
    length = norm(vec)
    return vec/length if length > 0 else vec


def distance(v1: ndarray, v2: ndarray) -> float:
    """求两个向量之间的距离

    @param v1:
    @param v2:
    @return:
    """
    return norm(v2-v1)


def vector_degrees(vec: ndarray) -> float:
    """求向量相对x轴的角度

    @param vec:
    @return:
    """
    return math.degrees(math.atan2(vec[1], vec[0]))


def translate(vec: ndarray, delta: ndarray) -> ndarray:
    """位移向量

    @param vec:
    @param delta:
    @return:
    """
    return translate_matrix(delta)@vec


def translate_matrix(delta: ndarray) -> ndarray:
    """得到位移矩阵

    @param delta: 位移向量
    @return:
    """
    return array([[1, 0, delta[0]], [0, 1, delta[1]], [0, 0, 1]])


def rotate(vec: ndarray, rotate: float) -> ndarray:
    """旋转向量

    @param vec:
    @param rotate: 旋转角度
    @return:
    """
    return rotate_matrix(rotate)@vec


def rotate_matrix(rotate: float) -> ndarray:
    """得到旋转矩阵

    @param rotate:
    @return:
    """
    theta = math.radians(rotate)
    return array([[math.cos(theta), -math.sin(theta), 0], [math.sin(theta), math.cos(theta), 0], [0, 0, 1]])


def scale(vec: ndarray, scale_factor: float) -> ndarray:
    """放缩向量

    @param vec:
    @param scale_factor:
    @return:
    """
    return scale_matrix(scale_factor)@vec


def scale_matrix(scale_factor: float) -> ndarray:
    """得到放缩矩阵

    @param scale_factor:
    @return:
    """
    return array([[scale_factor, 0, 0], [0, scale_factor, 0], [0, 0, 1]])

