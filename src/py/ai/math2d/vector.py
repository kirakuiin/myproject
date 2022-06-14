"""向量运算"""

import functools
from numpy import *


def vector(x: float = 0.0, y: float = 0.0) -> ndarray:
    """向量

    @param x: x分量
    @param y: y分量
    @return:
    """
    return array([x, y, 0])


def position(x: float = 0, y: float = 0) -> ndarray:
    """位置

    @param x: x坐标
    @param y: y坐标
    @return:
    """
    return array([x, y, 1])


def as_vector(angle: float) -> ndarray:
    """将角度转化为单位向量

    @param angle:
    @return:
    """
    return rotate(vector(1, 0), angle)


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
    return value / abs(value)


@normalize.register
def _(vec: ndarray) -> ndarray:
    length = norm(vec)
    return vec / length if length > 0 else vec


def distance(v1: ndarray, v2: ndarray) -> float:
    """求两个向量之间的距离

    @param v1:
    @param v2:
    @return:
    """
    return norm(v2 - v1)


def as_degrees(vec: ndarray) -> float:
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
    return translate_matrix(delta) @ vec


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
    return rotate_matrix(rotate) @ vec


def rotate_matrix(rotate: float) -> ndarray:
    """得到旋转矩阵

    @param rotate:
    @return:
    """
    theta = math.radians(rotate)
    return array([[math.cos(theta), -math.sin(theta), 0], [math.sin(theta), math.cos(theta), 0], [0, 0, 1]])


def scale(vec: ndarray, factors: ndarray) -> ndarray:
    """放缩向量

    @param vec:
    @param factors:
    @return:
    """
    return scale_matrix(factors) @ vec


def scale_matrix(scale_factors: ndarray) -> ndarray:
    """得到放缩矩阵

    @param scale_factors:
    @return:
    """
    return array([[scale_factors[0], 0, 0], [0, scale_factors[1], 0], [0, 0, 1]])


class Transform(object):
    """变换信息"""

    def __init__(self, pos: ndarray = position(0, 0), rotate: float = 0.0, scales: ndarray = array((1.0, 1.0))):
        self.pos = pos  # 位置
        self.rotate = rotate  # 旋转角度
        self.scales = scales  # 放缩比例

    def __repr__(self):
        return '{}(pos: {}, rotate: {}, scale: {})'.format(
            self.__class__.__name__, self.pos, self.rotate, self.scales)

    def __iter__(self):
        return iter((self.pos, self.rotate, self.scales))

    def copy(self):
        """返回一个复制

        @return:
        """
        return Transform(*self)

    def combine(self, other):
        """组合两个变换, 生成一个新的变换

        @param other: 另一个变换
        @return: 新的变换
        """
        pos = self.get_transform_matrix(other.pos, other.rotate, other.scales) @ self.pos
        scales = other.scales*self.scales
        rotate = other.rotate + self.rotate
        return self.__class__(pos, rotate, scales)

    @staticmethod
    def get_transform_matrix(pos: ndarray, rotate: float, scales: ndarray):
        """根据偏移, 旋转, 缩放来获得变换矩阵

        @param pos: 偏移
        @param rotate: 旋转
        @param scales: 缩放
        @return: 3阶变换矩阵
        """
        base = eye(3)
        base = scale_matrix(scales) @ base
        base = rotate_matrix(rotate) @ base
        base = translate_matrix(pos) @ base
        return base
