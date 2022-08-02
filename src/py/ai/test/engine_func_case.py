"""引擎功能示例
"""

import case
import gameengine
import math2d
from gameengine import gameobject
from gameengine import camera
from gameengine import defines


@case.register_case(__name__)
class CameraClip(case.Case):
    """相机裁剪"""
    def init_case(self):
        self._circle = gameobject.Circle(100)
        self._circle.set_pos(400, 400)
        self._red_circle= gameobject.Circle(50)
        self._red_circle.set_pos(400, 400)
        self._red_circle.set_color(*defines.RED)
        self.add_child(self._circle)
        self.add_child(self._red_circle)
        self._add_camera()

    def _add_camera(self):
        w, h = gameengine.get_window_size()
        camera_lt = camera.Camera()
        camera_lt.set_output_area(0, h/2, w/2, h/2)
        camera_rb = camera.Camera()
        camera_rb.set_output_area(0.75*w, 0, w/4, h/4)
        gameengine.get_main_camera().set_output_area(w/4, h/4, w/2, h/2)
        gameengine.get_camera_mgr().add_camera(camera_lt)
        gameengine.get_camera_mgr().add_camera(camera_rb)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class TransformConvert(case.Case):
    """变换转化"""
    def init_case(self):
        self._circle = gameobject.Circle(10)
        self._circle.set_scale(3)
        self._circle.set_pos(400, 400)
        self._circle.set_rotate(90)
        self._circle1 = gameobject.Circle(10)
        self._circle1.set_pos(200, 200)
        self._circle1.set_rotate(-45)
        self._circle1.set_scale(2/3)
        self._circle2 = gameobject.Circle(10)
        self._circle2.set_scale(0.5)
        self._circle2.set_pos(100, 100)
        self.add_child(self._circle)
        self._circle.add_child(self._circle1)
        self._circle1.add_child(self._circle2)
        self._text_world = gameobject.FixSizeText()
        self._text_world.set_pos(-100, 0)
        self._text_local = gameobject.FixSizeText()
        self._text_local.set_pos(100, 0)
        self._circle2.add_child(self._text_world)
        self._circle2.add_child(self._text_local)
        self._is_calc = False

    def update(self, dt: float):
        if not self._is_calc:
            world_pos = gameengine.convert_to_world_space(self._circle1, self._circle2.get_pos())
            local_pos = gameengine.convert_to_local_space(self._circle1, self._circle2.get_world_pos())
            self._text_world.set_text('world: ({}, {})'.format(world_pos[0], world_pos[1]))
            self._text_local.set_text('local: ({}, {})'.format(local_pos[0], local_pos[1]))
            self._is_calc = True
        self.quit_over_time(5)