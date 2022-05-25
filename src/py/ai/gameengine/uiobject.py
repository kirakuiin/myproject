"""可绘制对象基类
"""

import weakref

import pygame
import math2d

from . import global_vars
from . import defines


class Transform(object):
    """变换信息"""
    def __init__(self, pos: math2d.ndarray=math2d.position(0, 0), rotate: float=0, scale: float=1.0):
        self.pos = pos  # 位置
        self.rotate = rotate  # 旋转角度
        self.scale = float(scale)  # 放缩比例

    def __repr__(self):
        return '{}(pos: {}, rotate: {}, scale: {})'.format(
            self.__class__.__name__, self.pos, self.rotate, self.scale)

    def combine(self, other):
        """组合两个变换, 生成一个新的变换

        @param other: 另一个变换
        @return: 新的变换
        """
        pos = self.get_transform_matrix(other.pos, other.rotate, other.scale) @ self.pos
        scale = other.scale*self.scale
        rotate = other.rotate+self.rotate
        return self.__class__(pos, rotate, scale)

    @staticmethod
    def get_transform_matrix(pos: math2d.ndarray, rotate: float, scale: float):
        """根据偏移, 旋转, 缩放来获得变换矩阵

        @param pos: 偏移
        @param rotate: 旋转
        @param scale: 缩放
        @return: 3阶变换矩阵
        """
        base = math2d.eye(3)
        base = math2d.scale_matrix(scale)@base
        base = math2d.rotate_matrix(rotate)@base
        base = math2d.translate_matrix(pos)@base
        return base


class UIObject(object):
    """可显示ui对象类

    基于节点树模型进行实现
    """
    def __init__(self):
        self._parent = None   # 父节点
        self._children = []   # 孩子列表

        self._color = (255, 255, 255)  # 颜色
        self._local = Transform()  # 局部变换
        self._world = Transform()  # 世界变换
        self._dirty = True   # 脏标记
        self._order = 0  # 绘制顺序, 越大越后绘制
        self._is_visible = True  # 是否可见
        self._split_idx = 0  # 绘制先后分隔坐标

    def set_visible(self, is_visible: bool):
        self._is_visible = is_visible

    def get_visible(self) -> bool:
        return self._is_visible

    def set_order(self, order: int):
        self._order = order

    def get_order(self) -> int:
        return self._order

    def set_pos(self, x: float, y: float):
        self.set_pos_vec(math2d.position(x, y))

    def set_pos_vec(self, position: math2d.ndarray):
        self._dirty = True
        self._local.pos = position

    def get_pos(self) -> math2d.ndarray:
        return self._local.pos

    def get_world_pos(self) -> math2d.ndarray:
        return self._world.pos

    def set_color(self, r: int, g: int, b: int):
        self._color = (r, g, b)

    def get_color(self) -> tuple:
        return self._color

    def set_scale(self, scale: float):
        self._dirty = True
        self._local.scale = float(scale)

    def get_scale(self) -> float:
        return self._local.scale

    def set_rotate(self, rotate: float):
        self._dirty = True
        self._local.rotate = rotate

    def get_rotate(self) -> float:
        return self._local.rotate

    def get_world_rotate(self) -> float:
        return self._world.rotate

    def update(self, dt: float):
        """ui对象在每帧内的更新逻辑

        @param dt: 时间间隔
        @return:
        """
        pass

    def render(self, parent_transform: Transform, dirty: bool):
        dirty = self._dirty | dirty
        if dirty:
            self._world = self._local.combine(parent_transform)
            self._dirty = False
        self._render_order_lt_zero(dirty)
        self._render_self()
        self._render_order_ge_zero(dirty)

    def _render_order_lt_zero(self, dirty):
        for child in self._children[:self._split_idx]:
            child.render(self._world, dirty)

    def _render_self(self):
        self._is_visible and self.draw()

    def _render_order_ge_zero(self, dirty):
        for child in self._children[self._split_idx:]:
            child.render(self._world, dirty)

    def draw(self):
        """绘制自身的逻辑

        @return:
        """
        pass

    def get_children(self) -> list:
        return self._children

    def add_child(self, child, order=0):
        child.set_order(order)
        child._parent = weakref.ref(self)
        self._insert_child_keep_order(child)
        self._update_split_idx()

    def _insert_child_keep_order(self, child):
        for idx in range(len(self._children)-1, -1, -1):
            if child.get_order() >= self._children[idx].get_order():
                self._children.insert(idx+1, child)
                break
        else:
            self._children.insert(0, child)

    def _update_split_idx(self):
        self._split_idx = len(self._children)
        for idx, child in enumerate(self._children):
            if child.get_order() >= 0:
                self._split_idx = idx
                break

    def remove_child(self, child):
        if child in self._children:
            self._children.remove(child)
            child.set_parent(None)

    def set_parent(self, parent):
        self._dirty = True
        self._parent = parent

    def get_parent(self):
        if self._parent and self._parent():
            return self._parent()
        else:
            return None

    @staticmethod
    def get_screen_coord(pos: math2d.ndarray) -> list:
        """将坐标转换为屏幕坐标

        @param pos:
        @return: [x, y]
        """
        screen_pos = math2d.position(pos[0], defines.SCREEN_HEIGHT-pos[1])
        return [screen_pos[0], screen_pos[1]]


class Circle(UIObject):
    """圆环
    """
    def __init__(self, radius: float):
        super().__init__()
        self.radius = radius  # 半径

    def draw(self):
        pygame.draw.circle(global_vars.screen, self._color,
                           UIObject.get_screen_coord(self._world.pos),
                           self.radius*self._world.scale,)


class Triangle(UIObject):
    """正三角形"""
    def __init__(self, radius):
        super().__init__()
        self.radius = radius  # 从中心到顶点的距离

    def draw(self,):
        pygame.draw.polygon(global_vars.screen, self._color, self._calc_three_points())

    def _calc_three_points(self):
        top_vec = math2d.rotate(math2d.vector(0, self.radius*self._world.scale), self._world.rotate)
        left_vec = math2d.rotate(top_vec, 120)
        right_vec = math2d.rotate(left_vec, 120)
        return [UIObject.get_screen_coord(math2d.translate(self._world.pos, vec))
                for vec in [top_vec, left_vec, right_vec]]
