"""示例基类

一个示例代表一个可以运行的代码实体
"""

import collections
import pygame
import math2d
import gameengine
from gameengine import uiobject
from gameengine import global_vars


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
        self.register_handle(pygame.KEYUP, self.camera_control)

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
        print('结束, 运行时间: {:.2f}s'.format(self.get_runtime()))
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

    def get_runtime(self) -> float:
        """获得示例运行的总时间

        @return: float
        """
        return global_vars.run_time

    def quit_over_time(self, time: float=10):
        """经过x秒后关闭示例

        @param time: 关闭时间
        @return:
        """
        if self.get_runtime() > time:
            self.quit_engine()

    def update(self, dt):
        if self.get_runtime() > 5:
            self.quit_engine()

    def camera_control(self, event):
        """相机控制接口

        @param event:
        @return:
        """
        camera = gameengine.get_main_camera()
        pos, rotation, focus = camera.get_camera_param(True, True, True)
        if event.key == pygame.K_a:
            camera.set_lookat(pos+math2d.vector(-10, 0))
        elif event.key == pygame.K_d:
            camera.set_lookat(pos+math2d.vector(10, 0))
        elif event.key == pygame.K_w:
            camera.set_lookat(pos+math2d.vector(0, 10))
        elif event.key == pygame.K_s:
            camera.set_lookat(pos+math2d.vector(0, -10))
        elif event.key == pygame.K_q:
            camera.set_rotation(rotation+10)
        elif event.key == pygame.K_e:
            camera.set_rotation(rotation-10)
        elif event.key == pygame.K_z:
            camera.set_focus(focus*2)
        elif event.key == pygame.K_c:
            camera.set_focus(focus/2)


