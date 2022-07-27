"""阵型示例
"""

import case
import math2d
from . import defines
from . import formation


@case.register_case(__name__)
class MultiLevelSteeringCase(case.Case):
    """多级编队转向"""
    CHAR_NUM = 6
    def init_case(self):
        self._anchor = defines.DynamicObj(math2d.vector(30, 0), 90)
        self._anchor.set_constant(resistance_ratio=0)
        self._anchor.set_pos(400, 400)
        self._anchor.set_scale(0.3)
        self._anchor.set_visible(False)
        self.add_child(self._anchor)
        self._formation = formation.FormationManager(formation.TrianglePattern(100, 50*math2d.sqrt(3)))
        self._init_formation()

    def _init_formation(self):
        for i in range(3):
            char = self._create_circle_formation()
            self._formation.add_character(char)

    def _create_circle_formation(self):
        pattern = formation.DefenceCirclePattern(formation.StaticCharacter.RADIUS)
        mgr = formation.FormationManager(pattern)
        for i in range(self.CHAR_NUM):
            char = formation.StaticCharacter()
            mgr.add_character(char)
            self.add_child(char)
        return mgr

    def update(self, dt):
        self._formation.set_target(defines.Location(self._anchor.position(), self._anchor.rotation()))
        self._formation.update_slots()
        self.quit_over_time(5)
