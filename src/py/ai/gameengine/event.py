"""事件管理
"""

import collections
import pygame
import weakref

import math2d
from . import global_vars
from . import component
from . import util


class EventMgr(object):
    """事件管理器
    """
    def __init__(self):
        self._register_info = collections.defaultdict(list)  # 事件注册信息
        self._click_mgr = MouseEventMgr()  # 点击管理器
        self.register(pygame.MOUSEBUTTONUP, self._click_mgr.on_button_up)
        self.register(pygame.MOUSEMOTION, self._click_mgr.on_motion)
        self.register(pygame.MOUSEBUTTONDOWN, self._click_mgr.on_button_down)

    def register(self, event_type: int, handler):
        """注册事件处理函数

        @param event_type:
        @param handler:
        @return:
        """
        if handler not in self._register_info[event_type]:
            self._register_info[event_type].append(handler)

    def propagate(self):
        """传播事件

        @return:
        """
        for event in pygame.event.get():
            if event.type in self._register_info:
                for handler in self._register_info[event.type]:
                    handler(event)


class MouseEventMgr(object):
    """鼠标事件管理器"""
    def __init__(self):
        self._scene = weakref.proxy(global_vars.scene)  # 场景
        self._recv_motion_list = []  # 接受鼠标移动事件的对象列表
        self._cur_down_btn = None  # 当前按下的按钮

    def _get_order_list(self):
        return self._scene.get_render_order()

    def on_button_down(self, event):
        if self._is_valid_event(event):
            self._cur_down_btn = event.button
            self._propagate_down(event)

    def _propagate_down(self, event):
        for obj in self._get_order_list():
            if (obj() is None or not obj().get_component(component.Touch)
                    or not obj().get_component(component.Touch).is_touchable()):
                continue
            is_recv_motion = obj().on_begin(self._cur_down_btn, math2d.position(*util.get_window_coord(event.pos)))
            is_recv_motion and self._recv_motion_list.append(obj)
            if obj().get_component(component.Touch).is_swallow():
                break

    def _is_valid_event(self, event):
        if self._cur_down_btn is None:
            return event.button in [pygame.BUTTON_LEFT, pygame.BUTTON_RIGHT, pygame.BUTTON_MIDDLE]
        else:
            return event.button == self._cur_down_btn

    def on_motion(self, event):
        for obj in self._recv_motion_list:
            obj() and obj().get_component(component.Touch).on_motion(
                self._cur_down_btn, math2d.position(*util.get_window_coord(event.pos)),
                math2d.vector(event.rel[0], -event.rel[1]))

    def on_button_up(self, event):
        for obj in self._recv_motion_list:
            obj() and obj().get_component(component.Touch).on_end(
                self._cur_down_btn, math2d.position(*util.get_window_coord(event.pos)))
        self._cur_down_btn = None
        self._recv_motion_list.clear()