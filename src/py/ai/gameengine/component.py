"""组件"""
import weakref
import bisect
import operator

import math2d

from common.component import Component

from . import defines
from . import camera


class EngineComponent(Component):
    @property
    def game_object(self):
        """游戏对象"""
        return self.root

    def remove_component(self, comp_cls):
        assert comp_cls is not Transform, 'Transform组件无法被移除'
        super().remove_component(comp_cls)


class Transform(EngineComponent):
    """变换信息"""
    def __init__(self, game_object):
        super(Transform, self).__init__(game_object)
        self._transform = math2d.Transform()  # 变换信息
        self._pre_children = []  # order<0孩子列表
        self._post_children = []  # order>=0孩子列表
        self._parent_node = None   # 父节点
        self._order = 0  # 节点序

    @property
    def transform(self):
        return self._transform

    @property
    def children(self):
        return self._pre_children+self._post_children

    @property
    def pre_children(self):
        return self._pre_children

    @property
    def post_children(self):
        return self._post_children

    def set_order(self, order: int):
        self._order = order

    def get_order(self) -> int:
        return self._order

    def set_parent(self, parent):
        self._parent_node = weakref.ref(parent) if parent else None

    def get_parent(self):
        return self._parent_node and self._parent_node()

    def add_child(self, child, order=0):
        child.transform.set_parent(self.game_object)
        child.transform.set_order(order)
        children_list = self._get_list_by_order(order)
        bisect.insort(children_list, child, key=operator.methodcaller('get_order'))

    def _get_list_by_order(self, order):
        return self._pre_children if order < 0 else self._post_children

    def remove_child(self, child):
        children_list = self._get_list_by_order(child.transform.get_order())
        if child in children_list:
            children_list.remove(child)
            child.transform.set_parent(None)


class Render(EngineComponent):
    """渲染信息"""
    def __init__(self, game_object):
        super().__init__(game_object)
        self._transform = math2d.Transform()  # 世界变换
        self._color = defines.WHITE  # 颜色
        self._dirty = True   # 脏标记
        self._is_visible = True  # 是否可见
        self._watch_num = 0  # 观察编号

    @property
    def transform(self):
        return self._transform

    def set_dirty(self, is_dirty: bool):
        self._dirty = is_dirty

    def set_color(self, r: int, g: int, b: int):
        self._color = (r, g, b)

    def get_color(self) -> tuple:
        return self._color

    def set_visible(self, is_visible: bool):
        self._is_visible = is_visible

    def is_visible(self) -> bool:
        return self._is_visible

    def set_watch_num(self, num: int, is_set_child=True):
        self._watch_num = num
        if is_set_child:
            for child in self.game_object.transform.children:
                render_com = child.get_component(Render)
                render_com and render_com.set_watch_num(num)

    def get_watch_num(self):
        return self._watch_num

    def render(self, parent_transform: math2d.Transform, dirty: bool, is_visible: bool, camera: camera.Camera):
        """渲染对象

        @param parent_transform: 父变换
        @param dirty: 脏标记
        @param is_visible: 父节点是否可见
        @param camera: 渲染用摄像机
        @return:
        """
        dirty = self._dirty or dirty
        is_visible = self._is_visible and is_visible
        self._set_touch_visible(is_visible)
        if dirty:
            self._transform = self.game_object.transform.transform.combine(parent_transform)
            self._dirty = False
        self._render_order_lt_zero(dirty, is_visible, camera)
        self._render_self(is_visible, camera)
        self._render_order_ge_zero(dirty, is_visible, camera)

    def _set_touch_visible(self, is_visible):
        touch = self.get_component_in_parent(Touch)
        touch and touch.set_finally_visible(is_visible)

    def _render_order_lt_zero(self, dirty, is_visible, camera):
        for child in self.game_object.transform.pre_children:
            render_com = child.get_component(Render)
            render_com and render_com.render(self._transform, dirty, is_visible, camera)

    def _render_self(self, is_visible, camera):
        if is_visible and self._watch_num == camera.get_watch_num():
            self.game_object.draw(camera.get_viewport_transform(self._transform))

    def _render_order_ge_zero(self, dirty, is_visible, camera):
        for child in self.game_object.transform.post_children:
            render_com = child.get_component(Render)
            render_com and render_com.render(self._transform, dirty, is_visible, camera)


class Touch(EngineComponent):
    """触控信息"""
    def __init__(self, game_object):
        super(Touch, self).__init__(game_object)
        self._is_swallow = False  # 是否吞噬点击
        self._is_touchable = True  # 是否可接触
        self._is_finally_visible = True  # 是否最终可见(受父节点影响)

    def set_finally_visible(self, is_visible):
        self._is_finally_visible = is_visible

    def set_touchable(self, is_touchable):
        self._is_touchable = is_touchable

    def is_touchable(self) -> bool:
        return self._is_touchable and self._is_finally_visible

    def set_swallow(self, is_swallow: bool):
        self._is_swallow = is_swallow

    def is_swallow(self) -> bool:
        return self._is_swallow

    def on_begin(self, btn: int, pos: math2d.ndarray) -> bool:
        """开始点击时调用此接口
        @return:
        """
        return True

    def on_motion(self, btn: int, pos: math2d.ndarray, delta: math2d.ndarray):
        """按住移动时调用此接口
        @return:
        """
        pass

    def on_end(self, btn: int, pos: math2d.ndarray):
        """释放鼠标时调用此接口
        @return:
        """
        pass
