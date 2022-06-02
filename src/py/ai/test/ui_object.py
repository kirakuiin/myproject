"""ui对象测试
"""

import case
import math2d
from gameengine import uiobject


@case.register_case(__name__)
class LinesCase(case.Case):
    """线体绘制"""
    def init_case(self):
        self._line = uiobject.Lines(math2d.position(120, 120), math2d.position(100, 140),
                                    math2d.position(180, 100), is_close=True)
        self._line.set_pos(100, 100)
        self.add_child(self._line)