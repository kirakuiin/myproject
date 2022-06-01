"""可绘制对象基类
"""

import weakref
import bisect

import pygame
import math2d

from . import global_vars
from . import camera
from . import defines


class UIObject(object):
    """可显示ui对象类

    基于节点树模型进行实现
    """
    def __init__(self):
        self._parent = None   # 父节点
        self._children = []   # 孩子列表

        self._color = (255, 255, 255)  # 颜色
        self._local = math2d.Transform()  # 局部变换
        self._world = math2d.Transform()  # 世界变换
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
        self._local.scales = math2d.array((scale, scale))

    def get_scale(self) -> math2d.ndarray:
        return self._local.scales

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

    def render(self, parent_transform: math2d.Transform, dirty: bool, camera: camera.Camera):
        """渲染对象

        @param parent_transform: 父变换
        @param dirty: 脏标记
        @param camera: 渲染用摄像机
        @return:
        """
        dirty = self._dirty | dirty
        if dirty:
            self._world = self._local.combine(parent_transform)
            self._dirty = False
        self._render_order_lt_zero(dirty, camera)
        self._render_self(camera)
        self._render_order_ge_zero(dirty, camera)

    def _render_order_lt_zero(self, dirty, camera):
        for child in self._children[:self._split_idx]:
            child.render(self._world, dirty, camera)

    def _render_self(self, camera):
        if self._is_visible:
            transform = camera.get_after_camera_transform(self._world)
            camera.is_in_camera_view(transform) and self.draw(camera.get_viewport_transform(transform))

    def _render_order_ge_zero(self, dirty, camera):
        for child in self._children[self._split_idx:]:
            child.render(self._world, dirty, camera)

    def draw(self, transform):
        """绘制自身的逻辑

        @param transform: 最终变换
        @return:
        """
        pass

    def get_children(self) -> list:
        return self._children

    def add_child(self, child, order=0):
        child.set_order(order)
        child._parent = weakref.ref(self)
        bisect.insort(self._children, child, key=lambda obj: obj.get_order())
        self._split_idx = bisect.bisect_left(self._children, 0, key=lambda obj: obj.get_order())

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

    def draw(self, transform):
        pygame.draw.circle(global_vars.screen, self._color,
                           UIObject.get_screen_coord(transform.pos),
                           self.radius*transform.scales[0],)


class Triangle(UIObject):
    """正三角形"""
    def __init__(self, radius):
        super().__init__()
        self.radius = radius  # 从中心到顶点的距离

    def draw(self, transform):
        pygame.draw.polygon(global_vars.screen, self._color, self._calc_three_points(transform))

    def _calc_three_points(self, transform):
        top_vec = math2d.rotate(math2d.vector(0, self.radius*transform.scales[0]), transform.rotate)
        left_vec = math2d.rotate(top_vec, 120)
        right_vec = math2d.rotate(left_vec, 120)
        return [UIObject.get_screen_coord(math2d.translate(transform.pos, vec))
                for vec in [top_vec, left_vec, right_vec]]
