"""示例基类

一个示例代表一个可以运行的代码实体
"""

import collections
import pygame
import math2d
import gameengine
from gameengine import uiobject
from gameengine import global_vars
from gameengine import camera
from gameengine import defines


CaseContainer = collections.defaultdict(list)  # 示例容器


def get_case_container() -> collections.defaultdict:
    """返回示例容器

    @return:
    """
    return CaseContainer


def get_case_by_type(base_type: str) -> list:
    """返回指定类型的容器

    @param base_type: 类型
    @return: 指定类型的列表
    """
    return CaseContainer[base_type]


def get_all_type() -> list:
    """返回所有示例类型

    @return: 全部容器类型
    """
    return list(CaseContainer.keys())


def register_case(base_type):
    """注册示例到全局字典中

    @param base_type:
    @return:
    """
    def deco(cls):
        cls.BaseType = base_type
        CaseContainer[base_type].append(cls)
        return cls
    return deco


class Case(uiobject.UIObject):
    """示例

    每一个继承此类的对象都需要重写init_case
    """
    BaseType = ''  # 类型名称

    @classmethod
    def get_desc(cls) -> str:
        """返回描述

        @return: str
        """
        return '示例: {}[{}], 位于{}'.format(cls.__doc__, cls.__name__, cls.BaseType)

    def init_engine(self):
        """初始化引擎

        @return:
        """
        gameengine.init()
        self.register_handle(pygame.KEYUP, self.rotate_control)
        self.register_handle(pygame.MOUSEWHEEL, self.scale_control)
        self._init_camera_info()
        self._init_coord()

    def _init_camera_info(self):
        self._scale_info = uiobject.Text(20)
        self._center_info = uiobject.Text(20)
        self._rotate_info = uiobject.Text(20)
        self._scale_info.set_watch_num(1)
        self._scale_info.set_color(*defines.BLUE)
        self._rotate_info.set_watch_num(1)
        self._rotate_info.set_pos(0, 20)
        self._rotate_info.set_color(*defines.GREEN)
        self._center_info.set_watch_num(1)
        self._center_info.set_pos(0, 40)
        self._center_info.set_color(*defines.RED)
        self.add_child(self._scale_info)
        self.add_child(self._center_info)
        self.add_child(self._rotate_info)
        gameengine.get_camera_mgr().add_camera(camera.Camera(1, 1))
        self._update_camera_info()

    def _update_camera_info(self):
        cam = gameengine.get_main_camera()
        lookat, rotate, scale = cam.get_camera_param(True, True, True)
        self._scale_info.set_text('{:.2f}x'.format(scale))
        self._rotate_info.set_text('{}°'.format(rotate))
        self._center_info.set_text('{}, {}'.format(lookat[0], lookat[1]))

    def _init_coord(self):
        self._lookat = uiobject.Circle(2)
        self._lookat.set_watch_num(1)
        self._lookat.set_color(*defines.GREEN)
        self._lookat.set_pos(gameengine.get_window_width()/2, gameengine.get_window_height()/2)
        self._coord_lines = [uiobject.CoordLine(uiobject.CoordLine.LineType.VERTICAL, 0),
                             uiobject.CoordLine(uiobject.CoordLine.LineType.HORIZON, 0)]
        self.add_child(self._lookat)
        for node in self._coord_lines:
            self.add_child(node, -1)

    def start_engine(self):
        """启动引擎

        @return:
        """
        print('运行{}'.format(self.get_desc()))
        gameengine.get_scene().add_child(self)
        gameengine.start()

    def quit_engine(self):
        """结束引擎

        @return:
        """
        print('结束, 运行时间: {:.2f}s'.format(self.get_run_time()))
        gameengine.quit()

    def init_case(self):
        """初始化示例

        @return:
        """
        pass

    def register_handle(self, event_type: int, func):
        """注册事件监听处理

        @param event_type: 事件类型
        @param func: 回调函数
        @return:
        """
        gameengine.register_handle(event_type, func)

    def get_run_time(self) -> float:
        """获得示例运行的总时间

        @return: float
        """
        return global_vars.run_time

    def quit_over_time(self, time: float=10):
        """经过x秒后关闭示例

        @param time: 关闭时间
        @return:
        """
        if self.get_run_time() > time:
            self.quit_engine()

    def scale_control(self, event):
        """相机缩放控制

        @param event:
        @return:
        """
        camera = gameengine.get_main_camera()
        focus = camera.get_camera_param(focus=True)
        if event.y == 1:
            camera.set_focus(focus*2)
        else:
            camera.set_focus(focus/2)
        self._update_camera_info()

    def rotate_control(self, event):
        """相机旋转控制

        @param event:
        @return:
        """
        camera = gameengine.get_main_camera()
        rotation = camera.get_camera_param(rotation=True)
        if event.key == pygame.K_q:
            camera.set_rotation(rotation+10)
        elif event.key == pygame.K_e:
            camera.set_rotation(rotation-10)
        self._update_camera_info()

    def on_begin(self, btn: int, pos: math2d.ndarray) -> bool:
        if btn == pygame.BUTTON_RIGHT:
            return True
        else:
            return False

    def on_motion(self, btn: int, pos: math2d.ndarray, delta: math2d.ndarray):
        cam = gameengine.get_main_camera()
        camera_pos = cam.get_camera_param(lookat=True)
        cam.set_lookat(camera_pos-delta)
        self._update_camera_info()

