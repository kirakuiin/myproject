"""投射物射击示例
"""

import case
import math2d
from gameengine import defines as color
from . import defines
from . import algorithm


@case.register_case(__name__)
class FiringCase(case.Case):
    """无阻力射击"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector())
        self._character.set_pos(200, 400)
        self._character.set_constant(resistance_ratio=0, max_velocity=1000)
        self._target = defines.DynamicObj(math2d.vector())
        self._target.set_pos(600, 400)
        self._target.set_color(*color.GREEN)

        self.add_child(self._character)
        self.add_child(self._target)

        self._init_speed()

    def _init_speed(self):
        speed, gravity = 150, math2d.vector(0, -30)
        direction = algorithm.get_firing_solution(self._character.get_pos(), self._target.get_pos(), speed, gravity)
        if direction is not None:
            self._character.set_velocity(direction*speed)
            self._character.set_velocity_acc(gravity)

    def update(self, dt):
        if algorithm.is_close_enough(self._character.get_world_pos(), self._target.get_world_pos()):
            self.quit_engine()