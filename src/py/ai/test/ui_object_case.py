"""ui对象示例
"""

import case
import math2d

from gameengine import gameobject
from gameengine import defines


@case.register_case(__name__)
class Circle(case.Case):
    """圆环绘制"""
    def init_case(self):
        self._circle = gameobject.Circle(100)
        self._circle.set_pos(300, 300)
        self.add_child(self._circle)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class Triangle(case.Case):
    """三角绘制"""
    def init_case(self):
        self._triangle = gameobject.Triangle(100)
        self._triangle.set_pos(300, 300)
        self.add_child(self._triangle)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class Lines(case.Case):
    """线体绘制"""
    def init_case(self):
        self._line = gameobject.Lines(math2d.position(120, 120), math2d.position(500, 140),
                                    math2d.position(680, 400), is_close=True)
        self._line.set_pos(100, 100)
        self._line.set_rotate(45)
        self.add_child(self._line)

    def update(self, dt: float):
        self.quit_over_time(5)


@case.register_case(__name__)
class Text(case.Case):
    """文本绘制"""
    def init_case(self):
        self._text = gameobject.Text(bold=True, italic=True)
        self._text.set_pos(400, 400)
        self._text.set_text('hello world')
        self._text.set_color(*defines.RED)
        self._text.set_scale(3)
        self.add_child(self._text)

    def update(self, dt: float):
        self.quit_over_time(5)