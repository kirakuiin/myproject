"""ai实体
"""

import collections

import math2d
import time

from gameengine import uiobject
from gameengine import gameobject
from gameengine import defines
from move import defines as move_def


class Bullet(uiobject.UIObject):
    """子弹
    """
    RADIUS = 3
    SPEED = 500

    def __init__(self, direction, tag):
        super(Bullet, self).__init__()
        self._kine_com = move_def.KinematicComponent(self, direction*self.SPEED)
        self.add_component(self._kine_com)
        self._kine_com.set_constant(max_velocity=self.SPEED, resistance_ratio=0)
        self._tag = "tag"
        self._init_ui()

    def _init_ui(self):
        self._body = uiobject.Circle(self.RADIUS)
        self._body.set_color(*defines.ORANGE)
        self.add_child(self._body, 1)

    @property
    def tag(self):
        return self._tag

    def update(self, dt: float):
        self._kine_com.update(dt)


class AIEntity(uiobject.UIObject, move_def.KinematicInterface):
    """一个ai实体
    """
    RADIUS = 10
    MAX_BULLET = 5

    def __init__(self, tag="player"):
        super(AIEntity, self).__init__()
        self._kine_com = move_def.KinematicComponent(self)
        self.add_component(self._kine_com)
        self._tag = tag
        self._init_ui()

    def _init_ui(self):
        self._body = uiobject.Circle(self.RADIUS)
        self._body.set_color(*defines.RED)
        self._head = uiobject.Triangle(6)
        self._head.set_color(*defines.GREEN)
        self._head.set_pos(10, 0)
        self._head.set_rotate(-90)
        self.add_child(self._body, 1)
        self.add_child(self._head)

    @property
    def tag(self):
        return self._tag

    def update(self, dt: float):
        self._kine_com.update(dt)

    def position(self) -> math2d.ndarray:
        return self._kine_com.position()

    def rotation(self) -> float:
        return self._kine_com.rotation()

    def velocity(self) -> math2d.ndarray:
        return self._kine_com.velocity()

    def set_velocity(self, velocity: math2d.ndarray):
        self._kine_com.set_velocity(velocity)

    def max_velocity(self) -> float:
        return self._kine_com.max_velocity()

    def velocity_acc(self) -> math2d.ndarray:
        return self._kine_com.velocity_acc()

    def max_velocity_acc(self) -> float:
        return self._kine_com.max_velocity_acc()

    def set_velocity_acc(self, acc: math2d.ndarray):
        self._kine_com.set_velocity_acc(acc)

    def angular(self) -> float:
        return self._kine_com.angular()

    def max_angular(self) -> float:
        return self._kine_com.max_angular()

    def angular_acc(self) -> float:
        return self._kine_com.angular_acc()

    def max_angular_acc(self) -> float:
        return self._kine_com.max_angular_acc()

    def set_angular_acc(self, acc: float):
        self._kine_com.set_angular_acc(acc)

    def set_constant(self, max_velocity: float=None, max_velocity_acc: float=None,
                     max_angular: float=None, max_angular_acc: float=None, resistance_ratio: float=None):
        self._kine_com.set_constant(max_velocity, max_velocity_acc, max_angular, max_angular_acc, resistance_ratio)


class BulletManager(gameobject.GameObject):
    """子弹管理器"""
    MAX_NUM = 10
    CREATE_SPLIT = 1

    def __init__(self, root):
        super(BulletManager, self).__init__(root)
        self._bullet_list = collections.deque([], self.MAX_NUM)
        self._last_time = 0

    def create_bullet(self, pos, direction, tag):
        cur_time = time.time()
        if cur_time - self._last_time > 0.5:
            bullet = Bullet(direction, tag)
            bullet.set_pos_vec(pos)
            self.game_object.add_child(bullet)
            self._bullet_list.append(bullet)
            self._last_time = cur_time

    def check_collision(self, tar):
        for bullet in self._bullet_list:
            if is_collision(bullet.get_world_pos(), tar.get_world_pos(), 10):
                bullet.set_visible(False)
                return True
        else:
            return False


class AIEnemySpawner(object):
    """敌方ai生成器"""
    def __init__(self, root):
        self._root = root
        self._enemy = None

    def spawn(self):
        self._enemy = AIEntity("enemy")
        self._enemy.set_constant(resistance_ratio=0)
        self._enemy.set_rotate(math2d.randint(0, 360))
        self._enemy.set_velocity(math2d.randint(50, 80)*math2d.as_vector(self._enemy.get_rotate()))
        self._enemy.set_pos(math2d.randint(0, 800), math2d.randint(0, 800))
        self._root.add_child(self._enemy)
        return self._enemy

    def get_cur_enemy(self):
        return self._enemy


def is_collision(pos1, pos2, radius=3.0):
    """判断两个位置是否产生碰撞

    @param pos1:
    @param pos2:
    @param radius:
    @return:
    """
    return math2d.distance(pos1, pos2) < radius
