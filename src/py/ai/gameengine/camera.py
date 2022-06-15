"""简易2D摄像机"""
import gameengine
import math2d
import bisect
import pygame
import contextlib

from . import global_vars
from . import util
from common import signal


class Camera(object):
    """摄像机

    每个摄像机将会渲染一副新的图像到指定的位置
    """
    MAX_SCALE_VAL = 32  # 最大缩放倍数

    def __init__(self, priority: int=0, watch_num: int=0):
        self._output_area = (0, 0, util.get_window_width(), util.get_window_height())  # 输出区域
        self._watch_info = math2d.Transform(Camera.get_screen_center())   # 观察信息
        self._priority = priority  # 渲染优先级, 越高越后渲染
        self._watch_num = watch_num  # 观察编号
        self._is_enable = True  # 是否启用
        self._transform_signal = signal.Signal()  # 相机变换信号

    def set_watch_num(self, num: int):
        """设置观察编号

        @param num:
        @return:
        """
        self._watch_num = num

    def get_watch_num(self) -> int:
        """返回观察编号

        @return:
        """
        return self._watch_num

    def set_output_area(self, x: int, y: int, w: int, h: int):
        """设置摄像机输出位置

        @param x: x坐标
        @param y: y坐标
        @param w: 宽度
        @param h: 高度
        @return:
        """
        self._output_area = x, y, w, h

    @contextlib.contextmanager
    def clip_camera_area(self):
        """启动相机裁剪

        @return:
        """
        x, y, w, h = self._output_area
        rect = pygame.Rect(util.get_window_coord(math2d.position(x, h+y)), (w, h))
        global_vars.screen.set_clip(rect)
        yield None
        global_vars.screen.set_clip(None)

    def listen_transform(self, cb):
        """监听相机变换

        @param cb:
        @return:
        """
        self._transform_signal.connect(cb)

    def set_lookat(self, position: math2d.ndarray):
        """设置聚集位置

        @param position: 聚焦位置
        @return:
        """
        self._watch_info.pos = position
        self._transform_signal.emit(self._watch_info)

    def move(self, delta: math2d.ndarray):
        """根据相机当前变换在世界坐标下移动聚焦位置

        @param delta: 偏移量
        @return:
        """
        lookat, rotate, focus = self._watch_info
        delta = math2d.rotate(math2d.scale(delta, 1/focus), rotate)
        self.set_lookat(lookat+delta)

    def set_focus(self, factor: float):
        """设置聚焦倍数

        倍数位于0.03-32之间
        @param factor: 聚焦倍数
        @return:
        """
        factor = min(max(1/self.MAX_SCALE_VAL, factor), self.MAX_SCALE_VAL)
        self._watch_info.scales = math2d.array((factor, factor))
        self._transform_signal.emit(self._watch_info)

    def set_rotation(self, rotate: float=0.0):
        """设置相机旋转程度

        @param rotate: 旋转度
        @return:
        """
        self._watch_info.rotate = rotate
        self._transform_signal.emit(self._watch_info)

    def get_camera_transform(self) -> math2d.Transform:
        """获得相机参数
        @return:
        """
        return self._watch_info

    def get_priority(self) -> int:
        """返回渲染优先级

        @return:
        """
        return self._priority

    def set_enable(self, is_enable: bool):
        """设置是否启用

        @param is_enable:
        @return:
        """
        self._is_enable = is_enable

    def is_enable(self) -> bool:
        """摄像机是否启用

        @return: bool
        """
        return self._is_enable

    def get_viewport_transform(self, transform: math2d.Transform):
        """返回视口变换

        @param transform:
        @return:
        """
        x, y, w, h = self._output_area
        transform = self._get_after_camera_transform(transform)
        return transform.combine(math2d.Transform(
            math2d.position(x, y), scales=math2d.array((w/util.get_window_width(), h/util.get_window_height()))))

    def _get_after_camera_transform(self, world_transform: math2d.Transform) -> math2d.Transform:
        transform = self._to_camera_coord(world_transform)
        transform = self._apply_camera_transform(transform)
        transform = self._to_origin_coord(transform)
        return transform

    def _to_camera_coord(self, world_transform):
        return world_transform.combine(math2d.Transform(-self._watch_info.pos))

    def _apply_camera_transform(self, transform):
        return transform.combine(math2d.Transform(rotate=-self._watch_info.rotate, scales=self._watch_info.scales))

    def _to_origin_coord(self, transform):
        return transform.combine(math2d.Transform(math2d.vector(*util.get_window_size())/2))

    def get_view_rect(self) -> tuple:
        """返回观察矩形

        @return:
        """
        size = math2d.scale(math2d.vector(*gameengine.get_window_size()), 1/self._watch_info.scales)
        origin = self._watch_info.pos-size/2
        return origin, size

    @staticmethod
    def get_screen_center() -> math2d.ndarray:
        """返回摄像机中心屏幕坐标
        @return:
        """
        return math2d.position(util.get_window_width()/2, util.get_window_height()/2)


class CameraMgr(object):
    """摄像机管理器

    负责控制所有的相机
    """
    def __init__(self):
        self._camera_list = []  # 摄像机列表
        self._main_camera = Camera()  # 设置主相机
        self.add_camera(self._main_camera)

    def get_main_camera(self) -> Camera:
        """获得主相机

        @return:
        """
        return self._main_camera

    def add_camera(self, camera: Camera):
        """增加相机

        @param camera:
        @return:
        """
        bisect.insort(self._camera_list, camera, key=lambda obj: obj.get_priority())

    def remove_camera(self, camera: Camera):
        """移除相机

        @param camera:
        @return:
        """
        if camera in self._camera_list:
            self._camera_list.remove(camera)

    def render_all_camera(self):
        """渲染所有启用的相机画面

        @return:
        """
        for camera in self._camera_list:
            if camera.is_enable():
                with camera.clip_camera_area():
                    global_vars.scene.render(math2d.Transform(), False, camera)
