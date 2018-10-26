"""贪吃蛇

提高分数
控制蛇吃掉苹果会增加长度并提高分数
撞到墙壁或者自身会导致游戏结束

"""
#pylint: disable=no-member,undefined-variable

import turtle as t
import abc

class Controller(metaclass=abc.ABCMeta):
    """控制器，用来和玩家或者ai交互"""

    def __init__(self):
        self.score = 0
        self.alive = True
        self.snake = None
        self.world = None

    def get_walls_hitbox(self):
        """获得墙壁的hitbox"""
        return [hitbox for wall in self.world.walls.values()
                for hitbox in wall]

    def get_apple_hitbox(self):
        """获得苹果的hitbox"""
        return [hitbox for apple in self.world.apples.values()
                for hitbox in apple]

    def get_other_snake_hitbox(self):
        """获得其他蛇的hitbox"""
        return [hitbox for snake in self.world.snakes.values()
                for hitbox in snake]

    def get_self_hitbox(self):
        """获得自身的hitbox"""
        return [hitbox for hitbox in self.snake.get_hitbox()]

    def to_up(self):
        """向上移动"""
        self.snake.to_up()

    def to_down(self):
        """向下移动"""
        self.snake.to_down()

    def to_left(self):
        """向左移动"""
        self.snake.to_left()

    def to_right(self):
        """向右移动"""
        self.snake.to_right()

    @abc.abstractmethod
    def setup(self):
        """初始化控制器"""
        pass

    @abc.abstractmethod
    def control(self):
        """实现蛇的控制逻辑"""
        pass


class Player(Controller):
    """玩家(接受标准输入)"""

    def setup(self):
        t.onkey(self.to_up, 'w')
        t.onkey(self.to_down, 's')
        t.onkey(self.to_left, 'a')
        t.onkey(self.to_right, 'd')

    def control(self):
        pass

class AI(Controller):
    """电脑"""

    def setup(self):
        pass

    def control(self):
        pass
