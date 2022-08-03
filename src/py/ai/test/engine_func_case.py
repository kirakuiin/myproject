"""引擎功能示例
"""

import case
import gameengine
from gameengine import uiobject
from gameengine import camera
from gameengine import timer
from gameengine import defines


@case.register_case(__name__)
class CameraClip(case.Case):
    """相机裁剪"""
    def init_case(self):
        self._circle = uiobject.Circle(100)
        self._circle.set_pos(400, 400)
        self._red_circle= uiobject.Circle(50)
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
        self._circle = uiobject.Circle(10)
        self._circle.set_scale(3)
        self._circle.set_pos(400, 400)
        self._circle.set_rotate(90)
        self._circle1 = uiobject.Circle(10)
        self._circle1.set_pos(200, 200)
        self._circle1.set_rotate(-45)
        self._circle1.set_scale(2/3)
        self._circle2 = uiobject.Circle(10)
        self._circle2.set_scale(0.5)
        self._circle2.set_pos(100, 100)
        self.add_child(self._circle)
        self._circle.add_child(self._circle1)
        self._circle1.add_child(self._circle2)
        self._text_world = uiobject.FixSizeText()
        self._text_world.set_pos(-100, 0)
        self._text_local = uiobject.FixSizeText()
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


@case.register_case(__name__)
class Timer(case.Case):
    """定时器"""
    def init_case(self):
        self._circle = uiobject.Circle(20)
        self._circle.set_pos(400, 400)
        self.add_child(self._circle)
        self._timer = timer.Timer.create(self._bigger, 0.5, 10)
        self._call_time = 0

    def _bigger(self):
        self._circle.set_scale(self._circle.get_scale()[0]+0.2)
        if self._call_time == 4:
            gameengine.set_speed(2.0)
        if self._call_time == 9:
            gameengine.set_speed(1.0)
        self._call_time += 1

    def update(self, dt: float):
        self.quit_over_time(5)
