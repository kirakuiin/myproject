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
    @classmethod
    def get_origin(cls):
        return cls(0, 0, 0)

    def __init__(self, q=0, r=0, s=0):
        assert q+r+s == 0, "六边形坐标总和必须为0"
        self._q = q
        self._r = r
        self._s = s

    def __repr__(self):
        return '{}({}, {}, {})'.format(self.__class__.__name__, self.q, self.r, self.s)

    def __add__(self, other):
        return Hex(self.q + other.q, self.r + other.r, self.s + other.s)

    def __sub__(self, other):
        return Hex(self.q - other.q, self.r - other.r, self.s - other.s)

    def __mul__(self, other):
        return Hex(self.q * other, self.r * other, self.s * other)

    def __rmul__(self, other):
        return self*other

    def __eq__(self, other):
        return self.q == other.q and self.r == other.r and self.s == other.s

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

        以30°(尖顶), 或0°(平顶)的角作为起点, 逆时针排列
        @return: list[Corner]
        """
        return [Corner(self.q+1, self.r, self.s), Corner(self.q, self.r-1, self.s), Corner(self.q, self.r, self.s+1),
                Corner(self.q-1, self.r, self.s), Corner(self.q, self.r+1, self.s), Corner(self.q, self.r, self.s-1)]

    def get_corner_index(self, corner) -> int:
        """返回顶点位于网格的下标, 以30°(尖顶), 或0°(平顶)的角作为起点

        @return:
        """
        assert corner in self.get_all_corner(), "此顶点的不是六边形的顶点"
        return self.get_all_corner().index(corner)


def hex_distance(hex_a: Hex, hex_b: Hex) -> int:
    """返回两个六边形之间的距离

    @param hex_a:
    @param hex_b:
    @return:
    """
    new_hex = hex_a - hex_b
    return (abs(new_hex.q) + abs(new_hex.r) + abs(new_hex.s)) // 2


class Corner(object):
    """代表六边形网格中的一个六边形的顶点

    每个角最多被三个六边形共享
    """
    TYPE_POSITIVE = 1  # 总和为1
    TYPE_NEGATIVE = -1  # 总和为-1

    def __init__(self, q, r, s):
        assert abs(q+r+s) == 1, '顶点坐标总和绝对值必须为1'
        self._q = q
        self._r = r
        self._s = s

    def __repr__(self):
        return '{}({}, {}, {})'.format(self.__class__.__name__, self.q, self.r, self.s)

    def __eq__(self, other):
        return self.q == other.q and self.r == other.r and self.s == other.s

    @property
    def q(self):
        return self._q

    @property
    def r(self):
        return self._r

    @property
    def s(self):
        return self._s

    @property
    def type(self):
        return self.TYPE_POSITIVE if sum([self.q, self.r, self.s]) == 1 else self.TYPE_NEGATIVE

    def get_all_adjacency_hex(self) -> list:
        """返回邻接的三个六边形坐标列表

        @return: list[Hex]
        """
        if self.type == self.TYPE_POSITIVE:
            return [Hex(self.q, self.r, self.s-1), Hex(self.q-1, self.r, self.s), Hex(self.q, self.r-1, self.s)]
        else:
            return [Hex(self.q+1, self.r, self.s), Hex(self.q, self.r+1, self.s), Hex(self.q, self.r, self.s+1)]

    def get_all_adjacency_corner(self) -> list:
        """获得邻接的三个顶点

        @return: list[Corner]
        """
        if self.type == self.TYPE_POSITIVE:
            return [Corner(self.q, self.r-1, self.s-1),
                    Corner(self.q-1, self.r-1, self.s), Corner(self.q-1, self.r, self.s-1)]
        else:
            return [Corner(self.q+1, self.r+1, self.s), Corner(self.q+1, self.r, self.s+1),
                    Corner(self.q, self.r+1, self.s+1)]


# 朝向, fx代表变换矩阵, bx代表逆矩阵
Orientation = collections.namedtuple("Orientation", ["f0", "f1", "f2", "f3", "b0", "b1", "b2", "b3", "start_angle"])

# 尖顶六边形
Pointy = Orientation(math.sqrt(3), math.sqrt(3)/2, 0, -3/2, math.sqrt(3)/3, 1/3, 0, -2/3, 30)
# 平顶六边形
Flat = Orientation(3/2, 0, -math.sqrt(3)/2, -math.sqrt(3), 2/3, 0, -1/3, -math.sqrt(3)/3, 0)


# 尖顶方向
class PointyDirection(object):
    EAST=0
    NORTHEAST=1
    NORTHWEST=2
    WEST=3
    SOUTHWEST=4
    SOUTHEAST=5


# 平顶方向
class FlatDirection(object):
    SOUTHEAST=0
    NORTHEAST=1
    NORTH=2
    NORTHWEST=3
    SOUTHWEST=4
    SOUTH=5

# 方向列表
Directions = [0, 1, 2, 3, 4, 5]


class HexLayout(object):
    """决定hex坐标转换到平面坐标的规则
    """
    def __init__(self, size=vector(20, 20), origin=position(0, 0), layout=Pointy):
        self.size = size  # 六边形的大小
        self.origin = origin  # 原点
        self.orientation = layout  # 使用的布局


# 根据方向得到六边形
def hex_direction(direction: int) -> Hex:
    assert direction in Directions, "六边形方向无效"
    dir_list = [Hex(1, 0, -1), Hex(1, -1, -0), Hex(0, -1, 1),
                Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)]
    return dir_list[direction]


# 构建六边形环
def get_ring(center: Hex, radius: int) -> list:
    assert radius > 0, "环的半径必须大于0"
    result = []
    hex = center+hex_direction(PointyDirection.SOUTHWEST)*radius
    for dir in Directions:
        for i in range(radius):
            hex += hex_direction(dir)
            result.append(hex)
    return result


# 生成螺旋
def get_spiral_ring(center: Hex, radius: int) -> list:
    result = [center]
    for r in range(1, radius):
        result.extend(get_ring(center, r))
    return result


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


def corner_to_pixel(layout: HexLayout, coord: Corner):
    """将顶点坐标转化为屏幕坐标

    @param layout:
    @param coord:
    @return:
    """
    base_hex = coord.get_all_adjacency_hex()[0]
    index = base_hex.get_corner_index(coord)
    angle = math.radians(layout.orientation.start_angle + index*60)
    offset = vector(layout.size[0]*math.cos(angle), layout.size[1]*math.sin(angle))
    center = hex_to_pixel(layout, base_hex)
    return center+offset


def get_hex_corner_pixel(layout:HexLayout, coord: Hex) -> list:
    """返回六边形所有的顶点坐标

    @param layout:
    @param coord:
    @return:
    """
    result = []
    for corner in coord.get_all_corner():
        result.append(corner_to_pixel(layout, corner))
    return result
