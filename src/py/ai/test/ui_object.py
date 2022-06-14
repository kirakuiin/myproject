"""ui对象测试
"""

import case
import gameengine
import math2d

from gameengine import uiobject
from gameengine import defines


@case.register_case(__name__)
class CircleCase(case.Case):
    """圆环绘制"""
    def init_case(self):
        self._circle = uiobject.Circle(100)
        self._circle.set_pos(300, 300)
        self.add_child(self._circle)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class TriangleCase(case.Case):
    """三角绘制"""
    def init_case(self):
        self._triangle = uiobject.Triangle(100)
        self._triangle.set_pos(300, 300)
        self.add_child(self._triangle)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class LinesCase(case.Case):
    """线体绘制"""
    def init_case(self):
        self._line = uiobject.Lines(math2d.position(120, 120), math2d.position(100, 140),
                                    math2d.position(180, 100), is_close=True)
        self._line.set_pos(100, 100)
        self._line.set_rotate(90)
        self.add_child(self._line)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class TextCase(case.Case):
    """文本绘制"""
    def init_case(self):
        self._text = uiobject.Text(bold=True, italic=True)
        self._text.set_pos(400, 400)
        self._text.set_text('hello world')
        self._text.set_color(*defines.RED)
        self._text.set_scale(3)
        self.add_child(self._text)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class CoordLineCase(case.Case):
    """坐标线绘制"""
    def init_case(self):
        self._horizon_lines = [
            uiobject.CoordLine(uiobject.CoordLine.LineType.HORIZON, val)
            for val in range(0, 100, 20)
        ]
        self._vertlines = [
            uiobject.CoordLine(uiobject.CoordLine.LineType.VERTICAL, val)
            for val in range(0, 100, 20)
        ]
        for node in self._horizon_lines+self._vertlines:
            self.add_child(node)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class CoordSystemCase(case.Case):
    """坐标系统绘制"""
    def init_case(self):
        self._system = uiobject.CoordSystem(gameengine.get_main_camera(), 300)
        self.add_child(self._system)

    def update(self, dt: float):
        self.quit_over_time(5)
