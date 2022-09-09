"""数学函数示例
"""

import case
import math2d

from gameengine import uiobject
from gameengine import defines


@case.register_case(__name__)
class HexGrid(case.Case):
    """六边形绘制"""
    def init_case(self):
        self._layout = math2d.HexLayout(math2d.vector(30, 30), math2d.position(400, 400))
        self._grid = [
            math2d.Hex(0, 0, 0), math2d.Hex(1, 0, -1), math2d.Hex(1, -1, 0), math2d.Hex(0, -1, 1),
            math2d.Hex(-1, 0, 1), math2d.Hex(-1, 1, 0), math2d.Hex(0, 1, -1)
        ]
        self._lines = []
        for hex in self._grid:
            self._draw_hex(hex)

    def _draw_hex(self, hex: math2d.Hex):
        points = math2d.get_hex_corner_pixel(self._layout, hex)
        lineObj = uiobject.Lines(points[1], *points[2:], is_close=True)
        lineObj.set_pos_vec(points[0])
        lineObj.set_color(*defines.PURPLE)
        self._lines.append(lineObj)
        self.add_child(lineObj)

    def update(self, dt: float):
        self.quit_over_time(5)
