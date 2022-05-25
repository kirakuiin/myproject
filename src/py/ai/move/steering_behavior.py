"""转向行为
"""

import case
import math2d
from . import defines
from . import algorithm


@case.register_case(__name__)
class SteeringSeekCase(case.Case):
    """转向寻找"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(100, 0))
        self._character.set_pos(400, 300)
        self._character.max_speed= 1000
        self._character.resist_ratio = 20
        self._target = defines.StaticObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.acceleration = algorithm.get_acceleration(
            self._character, self._target, 100)
        if math2d.distance(self._character.get_world_pos(), self._target.get_world_pos()) < 10:
            self.quit_engine()


@case.register_case(__name__)
class SteeringArriveCase(case.Case):
    """转向到达"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(-80, 100))
        self._character.set_pos(100, 100)
        self._target = defines.StaticObj(math2d.vector(0, 0))
        self._target.set_pos(300, 300)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.acceleration = algorithm.get_acc_with_brake(
            self._character.get_world_pos(), self._target.get_world_pos(),
            self._character.velocity)
        if math2d.distance(self._character.get_world_pos(), self._target.get_world_pos()) < 10:
            self.quit_engine()


@case.register_case(__name__)
class SteeringAlignCase(case.Case):
    """转向对齐"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(0, 0))
        self._character.set_pos(300, 200)
        self._character.set_scale(5)
        self._target = defines.KinematicObj(math2d.vector(0, 0))
        self._target.set_pos(300, 200)
        self._target.set_rotate(550)
        self._target.set_scale(5)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.angular_acc = algorithm.get_rotate_with_bake(self._character.get_world_rotate(),
                                                                     self._target.get_world_rotate(), self._character.angular)
        if abs(math2d.angle_delta(self._character.get_world_rotate(), self._target.get_world_rotate())) < 3:
            self.quit_engine()


@case.register_case(__name__)
class SteeringVelocityMatchCase(case.Case):
    """转向速度匹配"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(-50, -50))
        self._character.set_pos(300, 200)
        self._character.resist_ratio = 0
        self._target = defines.KinematicObj(math2d.vector(100, 0))
        self._target.set_pos(100, 200)
        self._target.resist_ratio = 0

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.acceleration = algorithm.get_match_speed(self._character.velocity, self._target.velocity, 50)
        if math2d.distance(self._character.velocity, self._target.velocity) < 1:
            self.quit_engine()
