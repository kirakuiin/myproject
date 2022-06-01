"""简易2D摄像机"""

import math2d
import bisect

from . import global_vars
from . import util


class Camera(object):
    """摄像机

    每个摄像机将会渲染一副新的图像到指定的位置
    """
    def __init__(self, priority: int=0):
        self._output_area = (0, 0, util.get_window_width(), util.get_window_height())  # 输出区域
        self._watch_info = math2d.Transform(math2d.position(util.get_window_width()/2, util.get_window_height()/2))   # 观察信息
        self._priority = priority  # 渲染优先级, 越高越后渲染
        self._is_enable = True  # 是否启用

    def set_output_area(self, x: int, y: int, w: int, h: int):
        """设置摄像机输出位置

        @param x: x坐标
        @param y: y坐标
        @param w: 宽度
        @param h: 高度
        @return:
        """
        self._output_area = x, y, w, h

    def set_lookat(self, position: math2d.ndarray):
        """设置聚集位置

        @param position: 聚焦位置
        @return:
        """
        self._watch_info.pos = position

    def set_focus(self, factor: float):
        """设置聚焦倍数

        @param factor: 聚焦倍数
        @return:
        """
        self._watch_info.scales = math2d.array((factor, factor))

    def set_rotation(self, rotate: float=0.0):
        """设置相机旋转程度

        @param rotate: 旋转度
        @return:
        """
        self._watch_info.rotate = rotate

    def get_camera_param(self, lookat=True, rotation=False, focus=False) -> tuple:
        """获得相机参数

        @param lookat: 是否返回位置
        @param rotation: 是否返回旋转
        @param focus: 是否返回聚焦倍数
        @return:
        """
        result = []
        lookat and result.append(self._watch_info.pos)
        rotation and result.append(self._watch_info.rotate)
        focus and result.append(self._watch_info.scales[0])
        return tuple(result)

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

    def get_after_camera_transform(self, world_transform: math2d.Transform) -> math2d.Transform:
        """获得经过摄像机处理过的变换

        @param world_transform: 世界变换
        @return: 处理后变换
        """
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

    def get_viewport_transform(self, transform: math2d.Transform) -> bool:
        """返回视口变换

        @param transform:
        @return:
        """
        x, y, w, h = self._output_area
        return transform.combine(math2d.Transform(math2d.position(x, y),
                                                  scales=math2d.array((w/util.get_window_width(), h/util.get_window_height()))))

    @staticmethod
    def is_in_camera_view(transform: math2d.Transform) -> bool:
        """输出对象是否位于相机视野中

        @param transform:
        @return:
        """
        return 0 <= transform.pos[0] <= util.get_window_width() and 0 <= transform.pos[1] <= util.get_window_height()


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
                global_vars.scene.render(math2d.Transform(), False, camera)
