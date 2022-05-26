"""运动学移动算法
"""

import case
import math2d
from . import defines
from . import algorithm


@case.register_case(__name__)
class KinematicSeekCase(case.Case):
    """运动学寻找"""
    def init_case(self):
        self._character = defines.StaticObj(math2d.vector(50, 50))
        self._character.set_pos(50, 50)
        self._target = defines.StaticObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)
        self._target.set_color(0, 255, 0)
        self._speed = 100

        self.add_child(self._character)
        self.add_child(self._target)

    def get_velocity(self):
        direction = math2d.normalize(self._target.get_world_pos() - self._character.get_world_pos())
        velocity = direction * self._speed
        return velocity

    def update(self, dt):
        self._character.velocity = self.get_velocity()
        if algorithm.is_close_enough(self._character, self._target):
            self.quit_engine()


@case.register_case(__name__)
class KinematicWanderCase(case.Case):
    """运动学漫游"""
    def init_case(self):
        self._character = defines.StaticObj(math2d.vector(0, 0))
        self._character.set_pos(400, 400)
        self._speed = 50
        self._angular = 15
        self._rotation = 0

        self.add_child(self._character)

    def get_velocity(self):
        self._rotation += self._angular*math2d.rand_bio()
        rad = math2d.radians(self._rotation)
        return self._speed * math2d.vector(math2d.cos(rad), math2d.sin(rad))

    def update(self, dt):
        self._character.velocity = self.get_velocity()
        self.quit_over_time()
