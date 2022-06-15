"""转向行为
"""

import case
import math2d
from gameengine import uiobject
from gameengine import defines as color
from . import defines
from . import algorithm


@case.register_case(__name__)
class SteeringSeekCase(case.Case):
    """转向寻找"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(100, 0))
        self._character.set_pos(400, 300)
        self._character.set_constant(max_velocity=1000, resistance_ratio=20)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(algorithm.get_seek_acc(self._character.position(), self._target.position(), 100))
        if algorithm.is_close_enough(self._character.position(), self._target.position(), 10):
            self.quit_engine()


@case.register_case(__name__)
class SteeringArriveCase(case.Case):
    """转向到达"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(-80, 100))
        self._character.set_pos(100, 100)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(300, 300)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(algorithm.get_arrive_acc(self._character, self._target))
        if algorithm.is_close_enough(self._character.position(), self._target.position(), 10):
            self.quit_engine()


@case.register_case(__name__)
class SteeringAlignCase(case.Case):
    """转向对齐"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 0))
        self._character.set_pos(300, 200)
        self._character.set_scale(5)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(300, 200)
        self._target.set_rotate(550)
        self._target.set_scale(5)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_angular_acc(algorithm.get_align_acc(self._character, self._target.rotation()))
        if abs(math2d.angle_delta(self._character.rotation(), self._target.rotation())) < 3:
            self.quit_engine()


@case.register_case(__name__)
class SteeringVelocityMatchCase(case.Case):
    """转向速度匹配"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(-50, -50))
        self._character.set_pos(300, 200)
        self._character.set_constant(resistance_ratio=0)
        self._target = defines.DynamicObj(math2d.vector(100, 0))
        self._target.set_pos(100, 200)
        self._target.set_constant(resistance_ratio=0)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(algorithm.get_match_acc(self._character.velocity(), self._target.velocity(), 50))
        if math2d.distance(self._character.velocity(), self._target.velocity()) < 1:
            self.quit_engine()


@case.register_case(__name__)
class SteeringChaseCase(case.Case):
    """转向追逐"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 50))
        self._character.set_pos(400, 200)
        self._character.set_constant(resistance_ratio=20, max_velocity=120)
        self._target = defines.DynamicObj(math2d.vector(80, 0))
        self._target.set_pos(20, 400)
        self._target.set_constant(resistance_ratio=0)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(algorithm.get_chase_acc(self._character, self._target, 1))
        if math2d.distance(self._character.position(), self._target.position()) < 15:
            self.quit_engine()


@case.register_case(__name__)
class SteeringFaceCase(case.Case):
    """转向朝向"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 0), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(150)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)
        self._target.set_rotate(math2d.as_degrees(self._target.get_pos() - self._character.get_pos()))

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_angular_acc(algorithm.get_face_acc(self._character, self._target.position()))
        if abs(math2d.angle_delta(self._character.rotation(), self._target.rotation())) < 5:
            self.quit_engine()


@case.register_case(__name__)
class SteeringLookDirectlyCase(case.Case):
    """转向直视"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(30, 40), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(-150)
        self._character.set_constant(resistance_ratio=0)

        self.add_child(self._character)

    def update(self, dt):
        self._character.set_angular_acc(algorithm.get_look_direct_acc(self._character))
        if abs(math2d.angle_delta(math2d.as_degrees(self._character.velocity()),
                                  self._character.rotation())) < 5:
            self.quit_engine()


@case.register_case(__name__)
class SteeringWander(case.Case):
    """转向漫游"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 40))
        self._character.set_pos(400, 400)
        self._character.set_rotate(90)
        self._character.set_constant(max_angular_acc=100, max_velocity=10)

        self.add_child(self._character)

    def update(self, dt):
        output = algorithm.get_wander_acc(self._character)
        self._character.set_velocity_acc(output.velocity_acc)
        self._character.set_angular_acc(output.angular_acc)
        self.quit_over_time(10)


@case.register_case(__name__)
class SteeringPathFollowing(case.Case):
    """转向路径跟随"""
    PATH_OFFSET = 100

    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(40, 0))
        self._character.set_pos(120, 120)
        self._character.set_constant(max_velocity=80)
        self._path = defines.LinePath([math2d.position(100, 100), math2d.position(100, 350), math2d.position(350, 350),
                                       math2d.position(350, 500), math2d.position(500, 500), math2d.position(500, 750),
                                       math2d.position(600, 0)])
        self._near_point = uiobject.Circle(3)
        self._near_point.set_color(*color.ORANGE)

        self.add_child(self._character)
        self.add_child(self._path)
        self.add_child(self._near_point)

    def update(self, dt):
        near_point = self._path.get_line_point(self._character.get_pos(), self.PATH_OFFSET)
        self._near_point.set_pos_vec(near_point)
        self._character.set_velocity_acc(algorithm.get_seek_acc(self._character.position(), near_point, 200))
        self.quit_over_time()
