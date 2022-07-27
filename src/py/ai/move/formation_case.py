"""阵型示例
"""

import case
import math2d
from . import defines
from . import formation


@case.register_case(__name__)
class TwoLevelSteeringCase(case.Case):
    """两级编队转向"""
    def init_case(self):
        circle_pattern = formation.DefenceCirclePattern(formation.StaticCharacter.RADIUS)
        triangle_pattern = formation.TrianglePattern(100, 150)
        self._anchor = defines.DynamicObj(math2d.vector(30, 0))
        self._anchor.set_constant(resistance_ratio=0)
        self._anchor.set_pos(400, 400)
        self._anchor.set_scale(0.3)
        self.add_child(self._anchor)
        self._formation_cir= formation.FormationManager(circle_pattern)
        self._formation_tri = formation.FormationManager(triangle_pattern)
        self._init_formation()

    def _init_formation(self):
        for i in range(6):
            char = formation.StaticCharacter()
            self._formation_cir.add_character(char)
            self.add_child(char)
        for i in range(6):
            char = formation.StaticCharacter()
            self._formation_tri.add_character(char)
            self.add_child(char)

    def update(self, dt):
        self._formation_cir.update_slots(defines.Location(self._anchor.position(), self._anchor.rotation()))
        self._formation_tri.update_slots(defines.Location(self._anchor.position(), self._anchor.rotation()))
