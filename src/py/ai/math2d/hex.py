"""六边形库

坐标采用cube表示法(q, r, s)
"""


import collections

from . import position
from . import vector
from . import ndarray
from . import math


class Hex(object):
    """代表六边形网格中的一个六边形的位置"""
    def __init__(self, q=0, r=0, s=0):
        assert q+r+s == 0, "六边形坐标总和必须为0"
        self._q = q
        self._r = r
        self._s = s

    def __repr__(self):
        return '{}({}, {}, {})'.format(self.__class__.__name__, self._q, self._r, self._s)

    def __add__(self, other):
        return Hex(self._q + other._q, self._r + other._r, self._s + other._s)

    def __sub__(self, other):
        return Hex(self._q - other._q, self._r - other._r, self._s - other._s)

    def __mul__(self, other):
        return Hex(self._q * other, self._r * other, self._s * other)

    def __rmul__(self, other):
        return self*other

    def __eq__(self, other):
        return self._q == other._q and self._r == other._r and self._s == other._s

    @property
    def q(self):
        return self._q

    @property
    def r(self):
        return self._r

    @property
    def s(self):
        return self._s

    def get_all_corner(self) -> list:
        """返回六个顶点

        @return: list[Corner]
        """
        return [ Corner(self._q, self._r-1, self._s), Corner(self._q+1, self._r, self._s),
                 Corner(self._q, self._r, self._s-1), Corner(self._q, self._r+1, self._s),
                 Corner(self._q-1, self._r, self._s), Corner(self._q-1, self._r, self._s+1) ]


def hex_distance(hex_a: Hex, hex_b: Hex) -> int:
    """返回两个六边形之间的距离

    @param hex_a:
    @param hex_b:
    @return:
    """
    hex = hex_a-hex_b
    return (abs(hex.q) + abs(hex.r) + abs(hex.s)) // 2


class Corner(object):
    """代表六边形网格中的一个六边形的顶点

    每个角最多被三个六边形共享
    """
    def __init__(self, q, r, s):
        assert abs(q+r+s) == 1, '顶点坐标总和绝对值必须为1'
        self._q = q
        self._r = r
        self._s = s

    def __repr__(self):
        return '{}({}, {}, {})'.format(self.__class__.__name__, self._q, self._r, self._s)

    def __eq__(self, other):
        return self._q == other._q and self._r == other._r and self._s == other._s

    @property
    def q(self):
        return self._q

    @property
    def r(self):
        return self._r

    @property
    def s(self):
        return self._s



class Side(object):
    """代表六边形网格里的一个边

    每条边最多被两个六边形共享
    """
    pass


# 朝向, fx代表变换矩阵, bx代表逆矩阵
Orientation = collections.namedtuple("Orientation", ["f0", "f1", "f2", "f3", "b0", "b1", "b2", "b3", "start_angle"])

# 尖顶六边形
Pointy = Orientation(math.sqrt(3), math.sqrt(3)/2, 0, 3/2, math.sqrt(3)/3, -1/3, 0, 2/3, 30)
# 平顶六边形
Flat = Orientation(3/2, 0, math.sqrt(3)/2, math.sqrt(3), 2/3, 0, -1/3, math.sqrt(3)/3, 0)


class HexLayout(object):
    """决定hex坐标转换到平面坐标的规则
    """
    def __init__(self, size=vector(20, 20), origin=position(0, 0), layout=Pointy):
        self.size = size  # 六边形的大小
        self.origin = origin  # 原点
        self.orientation = layout  # 使用的布局


def hex_to_pixel(layout: HexLayout, coord: Hex) -> ndarray:
    """将网格坐标转换为屏幕坐标

    @param layout: 布局
    @param coord: 网格坐标
    @return:
    """
    matrix = layout.orientation
    x = (matrix.f0 * coord.q + matrix.f1 * coord.r) * layout.size[0]
    y = (matrix.f2 * coord.q + matrix.f3 * coord.r) * layout.size[1]
    return vector(x, y) + layout.origin


def get_hex_corner_pixel(layout:HexLayout, coord: Hex) -> list:
    """返回六边形所有的顶点坐标

    @param layout:
    @param coord:
    @return:
    """
    result = []
    center = hex_to_pixel(layout, coord)
    for i in range(6):
        angle = math.radians(layout.orientation.start_angle + i*60)
        offset = vector(layout.size[0]*math.cos(angle), layout.size[1]*math.sin(angle))
        result.append(center+offset)
    return result