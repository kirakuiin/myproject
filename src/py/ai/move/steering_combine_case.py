"""组合转向行为示例
"""

import case
import math2d
from gameengine import defines as color
from . import defines
from . import steering_behavior


@case.register_case(__name__)
class Flocking(case.Case):
    """蜂拥混合"""
    class Bird(object):
        def __init__(self, pos, velo):
            self.obj = defines.DynamicObj(velo)
            self.obj.set_constant(resistance_ratio=0)
            self.obj.set_pos_vec(pos)
            self.obj.set_rotate(math2d.as_degrees(velo))
            self.behavior = steering_behavior.BlendedSteering(self.obj)

        def update(self):
            acc = self.behavior.get_steering_output()
            self.obj.set_velocity_acc(acc.velocity_acc)
            self.obj.set_angular_acc(acc.angular_acc)

    class MasterBird(Bird):
        def init_behavior(self):
            self.obj.set_constant(max_velocity=30)
            self.obj.set_color(*color.ORANGE)
            self.behavior.add_behavior(steering_behavior.WanderBehavior(self.obj))

    class SlaveBird(Bird):
        def init_behavior(self, master_bird, other_birds):
            self.obj.set_constant(max_velocity_acc=300, max_angular=30)
            self.behavior.add_behavior(steering_behavior.VelocityMatchBehavior(self.obj, master_bird.obj), 1)
            self.behavior.add_behavior(steering_behavior.LookDirectBehavior(self.obj), 1)
            self.behavior.add_behavior(steering_behavior.ArriveBehavior(self.obj, master_bird.obj), 2)
            self.behavior.add_behavior(steering_behavior.SeparationBehavior(
                self.obj,[master_bird.obj]+[bird.obj for bird in other_birds]), 3)

    def init_case(self):
        self._birds = set()
        self._master_bird = self.MasterBird(math2d.position(400, 400), math2d.vector(10, 10))
        for i in range(5):
            x, y = math2d.randint(200, 600), math2d.randint(200, 600)
            bird = self.SlaveBird(math2d.position(x, y), math2d.vector())
            self.add_child(bird.obj)
            self._birds.add(bird)
        self.add_child(self._master_bird.obj)
        self._init_behavior()

    def _init_behavior(self):
        self._master_bird.init_behavior()
        for bird in self._birds:
            bird.init_behavior(self._master_bird, self._birds-{bird})

    def update(self, dt):
        self._master_bird.update()
        for bird in self._birds:
            bird.update()
        self.quit_over_time()


@case.register_case(__name__)
class Priority(case.Case):
    """优先混合"""
    def init_case(self):
        self._chararcter = defines.DynamicObj(math2d.vector(100, 0))
        self._chararcter.set_pos(300, 400)
        self._chararcter.set_constant(resistance_ratio=0, max_velocity_acc=200)
        self._target = defines.DynamicObj(math2d.vector())
        self._target.set_pos(500, 500)
        self.add_child(self._chararcter)
        self.add_child(self._target)
        self._init_behavior()

    def _init_behavior(self):
        seek_blend = steering_behavior.BlendedSteering(self._chararcter)
        look = steering_behavior.FaceBehavior(self._chararcter, self._target)
        seek = steering_behavior.SeekBehavior(self._chararcter, self._target)
        seek_blend.add_behavior(look)
        seek_blend.add_behavior(seek)
        self._priority = steering_behavior.PrioritySteering()
        sep = steering_behavior.SeparationBehavior(self._chararcter, {self._target}, decay_coe=200)
        self._priority.add_behavior(sep)
        self._priority.add_behavior(seek_blend)

    def update(self, dt):
        acc = self._priority.get_steering_output()
        self._chararcter.set_velocity_acc(acc.velocity_acc)
        self._chararcter.set_angular_acc(acc.angular_acc)
        self.quit_over_time()