"""阵型示例
"""
import case
import math2d
from gameengine import defines as color
from . import defines
from . import formation


@case.register_case(__name__)
class MultiLevelFormationCase(case.Case):
    """多级编队阵型"""
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


@case.register_case(__name__)
class FormationWithAssignmentCase(case.Case):
    """槽位分配阵型"""
    CHAR_NUM = 6

    def init_case(self):
        self._anchor = defines.DynamicObj(math2d.vector())
        self._anchor.set_constant(resistance_ratio=0)
        self._anchor.set_pos(400, 400)
        self._anchor.set_scale(0.3)
        self._anchor.set_visible(False)
        self.add_child(self._anchor)
        self._formation = formation.AssignFormationManager(formation.AdventurePattern(30))
        self._init_formation()

    def _init_formation(self):
        self._create_char_by_type(formation.TDHEnum.HEALER)
        self._create_char_by_type(formation.TDHEnum.TANK)
        self._create_char_by_type(formation.TDHEnum.DPS)
        self._create_char_by_type(formation.TDHEnum.TANK)

    def _create_char_by_type(self, type):
        char = formation.StaticCharacter()
        char.set_char_type(type)
        if type == formation.TDHEnum.TANK:
            char.set_color(*color.YELLOW)
        elif type == formation.TDHEnum.DPS:
            char.set_color(*color.BLUE)
        self.add_child(char)
        self._formation.add_character(char)

    def update(self, dt):
        self._formation.set_target(defines.Location(self._anchor.position(), self._anchor.rotation()))
        self._formation.update_slots()
        self.quit_over_time(5)
