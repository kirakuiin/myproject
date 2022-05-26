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
        self._character.acceleration = algorithm.get_seek_acc(self._character.get_world_pos(), self._target.get_world_pos(), 100)
        if algorithm.is_close_enough(self._character, self._target, 10):
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
        self._character.acceleration = algorithm.get_arrive_acc(self._character.get_world_pos(),
                                                                    self._target.get_world_pos(),
                                                                    self._character.velocity)
        if algorithm.is_close_enough(self._character, self._target, 10):
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
        self._character.angular_acc = algorithm.get_align_acc(self._character.get_world_rotate(),
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
        self._character.acceleration = algorithm.get_match_acc(self._character.velocity, self._target.velocity, 50)
        if math2d.distance(self._character.velocity, self._target.velocity) < 1:
            self.quit_engine()


@case.register_case(__name__)
class SteeringChaseCase(case.Case):
    """转向追逐"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(0, 50))
        self._character.set_pos(400, 200)
        self._character.resist_ratio = 20
        self._character.max_speed = 120
        self._target = defines.KinematicObj(math2d.vector(80, 0))
        self._target.set_pos(20, 400)
        self._target.resist_ratio = 0

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.acceleration = algorithm.get_chase_acc(
            self._character.get_world_pos(), self._target.get_world_pos(),
            self._character.velocity, self._target.velocity, 1)
        if math2d.distance(self._character.get_world_pos(), self._target.get_world_pos()) < 15:
            self.quit_engine()


@case.register_case(__name__)
class SteeringFaceCase(case.Case):
    """转向朝向"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(0, 0), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(150)
        self._target = defines.KinematicObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)
        self._target.set_rotate(math2d.vector_degrees(self._target.get_pos()-self._character.get_pos()))

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.angular_acc = algorithm.get_face_acc(
            self._character.get_pos(), self._character.get_rotate(), self._target.get_pos(),
            self._character.angular)
        if abs(math2d.angle_delta(self._character.get_rotate(), self._target.get_rotate())) < 5:
            self.quit_engine()


@case.register_case(__name__)
class SteeringLookDirectlyCase(case.Case):
    """转向直视"""
    def init_case(self):
        self._character = defines.KinematicObj(math2d.vector(30, 40), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(-150)
        self._character.resist_ratio = 0

        self.add_child(self._character)

    def update(self, dt):
        self._character.angular_acc = algorithm.get_look_direct_acc(
            self._character.get_rotate(), self._character.velocity, self._character.angular,
            max_angular=30
        )
        if abs(math2d.angle_delta(math2d.vector_degrees(self._character.velocity),
                                  self._character.get_rotate())) < 5:
            self.quit_engine()
