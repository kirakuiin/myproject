"""引擎功能示例
"""

import case
import gameengine
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