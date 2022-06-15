"""向量运算"""

from numpy import *

import functools


def vector(x: float = 0.0, y: float = 0.0) -> ndarray:
    """向量

    @param x: x分量
    @param y: y分量
    @return:
    """
    return array([x, y, 0], float)


def position(x: float = 0, y: float = 0) -> ndarray:
    """位置

    @param x: x坐标
    @param y: y坐标
    @return:
    """
    return array([x, y, 1], float)


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


class Line(object):
    """线段"""
    def __init__(self, begin, end):
        self.begin = begin
        self.end = end

    def __repr__(self):
        return '{}(b={}, e={})'.format(self.__class__.__name__, self.begin, self.end)

    def min_x(self):
        return min(self.begin[0], self.end[0])

    def min_y(self):
        return min(self.begin[1], self.end[1])

    def max_x(self):
        return max(self.begin[0], self.end[0])

    def max_y(self):
        return max(self.begin[1], self.end[1])

    def as_vector(self):
        return self.end - self.begin

    def length(self):
        return norm(self.as_vector())

    def normal(self):
        return normal(self.as_vector())

    def near_point(self, point: ndarray) -> ndarray:
        """直线上到点最近的一点

        @param point: 目标点
        @return: 直线上一点
        """
        vec1 = point-self.begin
        vec2 = self.as_vector()
        dis = dot(vec1, vec2)/self.length()
        if dis <= 0:
            return self.begin
        elif dis >= self.length():
            return self.end
        else:
            return self.begin+normalize(vec2)*dis


def as_vector(angle: float) -> ndarray:
    """将角度转化为单位向量

    @param angle:
    @return:
    """
    return rotate(vector(1, 0), angle)


def normal(vec: ndarray) -> ndarray:
    """获得单位法向量

    @param vec:
    @return:
    """
    return normalize(vector(vec[1], -vec[0]))


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


def is_parallel(a: Line, b: Line) -> bool:
    """判断两线平行
    """
    if norm(cross(a.as_vector(), b.as_vector())) == 0:
        return True
    else:
        return False


def is_intersection(a: Line, b: Line) -> bool:
    """判断线段相交

    重合不算相交
    @param a: 线段a
    @param b: 线段b
    @return:
    """
    if (a.max_x() < b.min_x()) or (a.max_y() < b.min_y()) or (b.max_x() < a.min_x()) or (b.max_y() < a.min_y()):
        return False
    A, B, C, D = a.begin, a.end, b.begin, b.end
    AB, AC, AD, CD, CA, CB = a.as_vector(), C-A, D-A, b.as_vector(), A-C, B-C
    if dot(cross(AB, AC), cross(AB, AD)) > 0 or dot(cross(CD, CA), cross(CD, CB)) > 0:
        return False
    if is_parallel(a, b):
        return False
    return True


def line_intersect(a: Line, b: Line) -> ndarray:
    """求得直线交点

    @param a:
    @param b:
    @return:
    """
    v_a, v_b = a.as_vector(), b.as_vector()
    t = norm(cross((b.begin-a.begin), v_b))/norm(cross(v_a, v_b))
    return a.begin+v_a*t