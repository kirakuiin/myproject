"""运动学移动算法示例
"""

import case
import math2d

from move import steering_behavior
from . import defines
from .. import ai_entity


@case.register_case(__name__)
class NormalDecisionTree(case.Case):
    """一般决策树"""
    def init_case(self):
        self._character = ai_entity.AIEntity()
        self._character.set_pos(400, 400)
        self._character.set_constant(max_velocity=200, max_velocity_acc=100, max_angular=200, max_angular_acc=100)
        self._bullet_mgr = ai_entity.BulletManager(self)
        self._spawner = ai_entity.AIEnemySpawner(self)
        self._enemy = self._spawner.spawn()
        self._decision = None

        self._create_decision()
        self.add_child(self._character)

    def _create_decision(self):
        align_decision = AlignDecision(FireAction(self._character, self._bullet_mgr), SeekAction(self._character, self._enemy), self._character, self._enemy)
        self._decision = NearDecision(align_decision, SeekAction(self._character, self._enemy), self._character, self._enemy)

    def update(self, dt):
        self._decision.make_decision()
        if self._bullet_mgr.check_collision(self._enemy):
            self._enemy = self._spawner.spawn()
            self._create_decision()
        self.quit_over_time()


class NearDecision(defines.Decision):
    def __init__(self, true, false, src: ai_entity.AIEntity, des: ai_entity.AIEntity):
        super(NearDecision, self).__init__(true, false)
        self._src_obj = src
        self._des_obj = des

    def test_value(self) -> bool:
        if math2d.distance(self._src_obj.position(), self._des_obj.position()) < 200:
            return True
        else:
            return False


class AlignDecision(defines.Decision):
    def __init__(self, true, false, src: ai_entity.AIEntity, des: ai_entity.AIEntity):
        super(AlignDecision, self).__init__(true, false)
        self._src_obj = src
        self._des_obj = des

    def test_value(self) -> bool:
        angle = math2d.as_degrees(self._des_obj.position()-self._src_obj.position()) % 360
        rot = self._src_obj.rotation() % 360
        if abs(angle-rot) < 3:
            return True
        else:
            return False


class FireAction(defines.Action):
    """开火动作"""
    def __init__(self, player: ai_entity.AIEntity, bullet_mgr: ai_entity.BulletManager):
        self._player = player
        self._mgr = bullet_mgr

    def make_decision(self):
        self._mgr.create_bullet(self._player.position(), math2d.as_vector(self._player.rotation()), self._player.tag)


class SeekAction(defines.Action):
    def __init__(self, player: ai_entity.AIEntity, enemy: ai_entity.AIEntity):
        self._player = player
        self._enemy = enemy
        self._behavior = None
        self._init_behavior()

    def _init_behavior(self):
        self._behavior = steering_behavior.BlendedSteering(self._player)
        self._behavior.add_behavior(steering_behavior.ChaseBehavior(self._player, self._enemy))
        self._behavior.add_behavior(steering_behavior.FaceBehavior(self._player, self._enemy))

    def make_decision(self):
        output = self._behavior.get_steering_output()
        self._player.set_velocity_acc(output.velocity_acc)
        self._player.set_angular_acc(output.angular_acc)