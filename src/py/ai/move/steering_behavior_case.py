"""转向行为示例
"""

import case
import math2d
from gameengine import uiobject
from gameengine import defines as color
from . import defines
from . import algorithm
from . import steering_behavior


@case.register_case(__name__)
class Seek(case.Case):
    """转向寻找"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(100, 0))
        self._character.set_pos(400, 300)
        self._character.set_constant(max_velocity=1000, resistance_ratio=20, max_velocity_acc=100)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)
        self._behavior = steering_behavior.SeekBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        if algorithm.is_close_enough(self._character.position(), self._target.position(), 10):
            self.quit_engine()


@case.register_case(__name__)
class Arrive(case.Case):
    """转向到达"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(-80, 100))
        self._character.set_pos(100, 100)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(300, 300)
        self._behavior = steering_behavior.ArriveBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        if algorithm.is_close_enough(self._character.position(), self._target.position(), 10):
            self.quit_engine()


@case.register_case(__name__)
class Align(case.Case):
    """转向对齐"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 0))
        self._character.set_pos(300, 200)
        self._character.set_scale(5)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(300, 200)
        self._target.set_rotate(550)
        self._target.set_scale(5)
        self._behavior = steering_behavior.AlignBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_angular_acc(self._behavior.get_steering_output().angular_acc)
        if abs(math2d.angle_delta(self._character.rotation(), self._target.rotation())) < 3:
            self.quit_engine()


@case.register_case(__name__)
class VelocityMatch(case.Case):
    """转向速度匹配"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(-50, -50))
        self._character.set_pos(300, 200)
        self._character.set_constant(resistance_ratio=0, max_velocity_acc=50)
        self._target = defines.DynamicObj(math2d.vector(100, 0))
        self._target.set_pos(100, 200)
        self._target.set_constant(resistance_ratio=0)
        self._behavior = steering_behavior.VelocityMatchBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        if math2d.distance(self._character.velocity(), self._target.velocity()) < 1:
            self.quit_engine()


@case.register_case(__name__)
class Chase(case.Case):
    """转向追逐"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 50))
        self._character.set_pos(400, 200)
        self._character.set_constant(resistance_ratio=20, max_velocity=120)
        self._target = defines.DynamicObj(math2d.vector(80, 0))
        self._target.set_pos(20, 400)
        self._target.set_constant(resistance_ratio=0)
        self._behavior = steering_behavior.ChaseBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        if math2d.distance(self._character.position(), self._target.position()) < 15:
            self.quit_engine()


@case.register_case(__name__)
class Face(case.Case):
    """转向朝向"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 0), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(150)
        self._target = defines.DynamicObj(math2d.vector(0, 0))
        self._target.set_pos(400, 400)
        self._target.set_rotate(math2d.as_degrees(self._target.get_pos() - self._character.get_pos()))
        self._behavior = steering_behavior.FaceBehavior(self._character, self._target)

        self.add_child(self._character)
        self.add_child(self._target)

    def update(self, dt):
        self._character.set_angular_acc(self._behavior.get_steering_output().angular_acc)
        if abs(math2d.angle_delta(self._character.rotation(), self._target.rotation())) < 5:
            self.quit_engine()


@case.register_case(__name__)
class LookDirectly(case.Case):
    """转向直视"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(30, 40), -30)
        self._character.set_pos(300, 350)
        self._character.set_rotate(-150)
        self._character.set_constant(resistance_ratio=0)
        self._behavior = steering_behavior.LookDirectBehavior(self._character)

        self.add_child(self._character)

    def update(self, dt):
        self._character.set_angular_acc(self._behavior.get_steering_output().angular_acc)
        if abs(math2d.angle_delta(math2d.as_degrees(self._character.velocity()),
                                  self._character.rotation())) < 5:
            self.quit_engine()


@case.register_case(__name__)
class Wander(case.Case):
    """转向漫游"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(0, 40))
        self._character.set_pos(400, 400)
        self._character.set_rotate(90)
        self._character.set_constant(max_angular_acc=100, max_velocity=10)
        self._behavior = steering_behavior.WanderBehavior(self._character)

        self.add_child(self._character)

    def update(self, dt):
        output = self._behavior.get_steering_output()
        self._character.set_velocity_acc(output.velocity_acc)
        self._character.set_angular_acc(output.angular_acc)
        self.quit_over_time(10)


@case.register_case(__name__)
class PathFollowing(case.Case):
    """转向路径跟随"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(40, 0))
        self._character.set_pos(120, 120)
        self._character.set_constant(max_velocity=80, max_velocity_acc=200)
        self._path = defines.LinePath([math2d.position(100, 100), math2d.position(100, 350), math2d.position(350, 350),
                                       math2d.position(350, 500), math2d.position(500, 500), math2d.position(500, 750),
                                       math2d.position(600, 0)])
        self._near_point = uiobject.Circle(3)
        self._near_point.set_color(*color.ORANGE)
        self._behavior = steering_behavior.PathFollowingBehavior(self._character, self._path)

        self.add_child(self._character)
        self.add_child(self._path)
        self.add_child(self._near_point)

    def update(self, dt):
        self._near_point.set_pos_vec(self._behavior.get_near_point())
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        self.quit_over_time()


@case.register_case(__name__)
class Separation(case.Case):
    """转向分离"""
    THRESHOLD = 100
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(100, 0))
        self._character.set_pos(200, 400)
        self._character.set_constant(max_velocity=80, resistance_ratio=0, max_velocity_acc=1000)
        self.add_child(self._character)
        self._init_targets()
        self._attach_repulsion_ring()
        self._behavior = steering_behavior.SeparationBehavior(self._character, self._target_list, self.THRESHOLD)

    def _init_targets(self):
        self._target_1 = defines.DynamicObj(math2d.vector())
        self._target_1.set_pos(400, 390)
        self.add_child(self._target_1)
        self._target_2 = defines.DynamicObj(math2d.vector())
        self._target_2.set_pos(370, 600)
        self.add_child(self._target_2)
        self._target_3 = defines.DynamicObj(math2d.vector())
        self._target_3.set_pos(600, 530)
        self.add_child(self._target_3)
        self._target_list = [self._target_1, self._target_2, self._target_3]

    def _attach_repulsion_ring(self):
        self._rings = []
        for child in self.transform.children:
            if isinstance(child, defines.DynamicObj):
                ring = uiobject.Circle(self.THRESHOLD, 1)
                ring.set_color(*color.ORANGE)
                child.add_child(ring)
                self._rings.append(ring)

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        self.quit_over_time()


@case.register_case(__name__)
class CollisionAvoidance(case.Case):
    """转向避免碰撞"""
    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(100, 0))
        self._character.set_pos(200, 400)
        self._character.set_constant(resistance_ratio=0, max_velocity_acc=1000, max_velocity=50)
        self.add_child(self._character)
        self._init_targets()
        self._behavior = steering_behavior.CollisionAvoidanceBehavior(self._character, self._target_list)

    def _init_targets(self):
        self._target_1 = defines.DynamicObj(math2d.vector(0, -50))
        self._target_1.set_pos(400, 605)
        self._target_1.set_constant(resistance_ratio=0)
        self.add_child(self._target_1)
        self._target_2 = defines.DynamicObj(math2d.vector(0, 40))
        self._target_2.set_pos(500, 100)
        self._target_2.set_constant(resistance_ratio=0)
        self.add_child(self._target_2)

        self._target_list = [self._target_1, self._target_2]

    def update(self, dt):
        self._character.set_velocity_acc(self._behavior.get_steering_output().velocity_acc)
        self.quit_over_time()


@case.register_case(__name__)
class ObstacleAvoidance(case.Case):
    """转向避免障碍物"""
    LOOK_AHEAD = 100
    AVOID_DIS = 50

    def init_case(self):
        self._character = defines.DynamicObj(math2d.vector(-80, 60))
        self._character.set_pos(400, 200)
        self._character.set_constant(resistance_ratio=0, max_velocity=100, max_velocity_acc=200)
        self.add_child(self._character)
        self._init_walls()
        self._behavior = steering_behavior.ObstacleAvoidanceBehavior(self._character, self._detector,
                                                                     self.LOOK_AHEAD, self.AVOID_DIS)

    def _init_walls(self):
        self._lines = [math2d.Line(math2d.position(100, 100), math2d.position(100, 600)),
                       math2d.Line(math2d.position(100, 600), math2d.position(200, 700)),
                       math2d.Line(math2d.position(200, 700), math2d.position(600, 700)),
                       math2d.Line(math2d.position(600, 700), math2d.position(700, 600))]
        self._detector = defines.CollisionDetector(self._lines)
        self._lines_objs = []
        for line in self._lines:
            line_obj = uiobject.Lines(line.end)
            line_obj.set_pos_vec(line.begin)
            self._lines_objs.append(line_obj)
            self.add_child(line_obj)

    def update(self, dt):
        acc = self._behavior.get_steering_output().velocity_acc
        math2d.norm(acc) > 0 and self._character.set_velocity_acc(acc)
        self._show_ray()
        self.quit_over_time()

    def _show_ray(self):
        begin = self._character.position()
        end = self._character.position()+math2d.normalize(self._character.velocity())*self.LOOK_AHEAD
        self._ray_line = uiobject.Lines(begin, end)
        self._ray_line.set_color(*color.GREEN)
        self._ray_line.set_pos_vec(begin)
        self.add_child(self._ray_line)


@case.register_case(__name__)
class Jump(case.Case):
    """转向跳跃"""
    GRAVITY = -30
    JUMP_SPEED = 150

    def init_case(self):
        self._jump_point = defines.JumpPoint(math2d.position(400, 400), math2d.position(600, 600))
        self._character = defines.DynamicObj(math2d.vector(30, 0))
        self._character.set_pos_vec(self._jump_point.takeoff_position)
        self._character.set_constant(resistance_ratio=0, max_velocity=180)
        self.add_child(self._character)
        self._init_jump_pad()
        self._behavior = steering_behavior.JumpBehavior(self._character, self._jump_point,
                                                        self.GRAVITY, self.JUMP_SPEED)

    def update(self, dt):
        acc = self._behavior.get_steering_output().velocity_acc
        self._character.set_velocity_acc(acc)
        if math2d.distance(self._character.position(), self._jump_point.landing_position) < 5:
            self.quit_engine()

    def _init_jump_pad(self):
        self._takeoff = uiobject.Lines(self._jump_point.takeoff_position+math2d.vector(10, 0), line_width=2)
        self._takeoff.set_pos_vec(self._jump_point.takeoff_position-math2d.vector(10, 0))
        self._landing = uiobject.Lines(self._jump_point.landing_position+math2d.vector(10, 0), line_width=2)
        self._landing.set_pos_vec(self._jump_point.landing_position-math2d.vector(10, 0))
        self.add_child(self._takeoff)
        self.add_child(self._landing)
