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
        self._target.set_color(*color.ORANGE)

        self.add_child(self._character)
        self.add_child(self._target)

        self._init_speed()

    def _init_speed(self):
        speed, gravity = 150, math2d.vector(0, -30)
        direction = algorithm.get_firing_solution(self._character.get_pos(), self._target.get_pos(), speed, gravity, False)
        self._character.set_velocity(direction*speed)
        self._character.set_velocity_acc(gravity)

    def update(self, dt):
        if algorithm.is_close_enough(self._character.get_world_pos(), self._target.get_world_pos()):
            self.quit_engine()


@case.register_case(__name__)
class FiringWithResistCase(case.Case):
    """有阻力射击"""
    Gravity = math2d.vector(0, -30)
    Speed = 200
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector())
        self._character.set_pos(200, 200)
        self._character.set_constant(resistance_ratio=20, max_velocity=1000)
        self._character.set_velocity_acc(self.Gravity)
        self._target = defines.DynamicObj(math2d.vector())
        self._target.set_pos(600, 400)
        self._target.set_color(*color.ORANGE)

        self.add_child(self._character)
        self.add_child(self._target)

        self._init_speed()

    def _init_speed(self):
        direction = algorithm.get_firing_solution_with_resist(
            self._character.get_pos(), self._target.get_pos(), self.Speed, self.Gravity, self.distance_to_target)
        self._character.set_velocity(direction*self.Speed)

    def update(self, dt):
        if algorithm.is_close_enough(self._character.position(), self._target.position()):
            self.quit_engine()

    def distance_to_target(self, direction):
        self._character.set_velocity(direction*self.Speed)
        distance = self._simulate(30)
        return distance

    def _simulate(self, time):
        dt = 1/100
        pos = self._character.get_pos()
        while time >= 0:
            time -= dt
            pos, _ = self._character.simulate(dt, pos, 0)
            if abs(pos[0]-self._target.get_pos()[0]) < 5:
                return pos[1]-self._target.get_pos()[1]
        return math2d.NoSolutionException()
