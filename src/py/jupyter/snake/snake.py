"""贪吃蛇

提高分数
控制蛇吃掉苹果会增加长度并提高分数
撞到墙壁或者自身会导致游戏结束

"""
#pylint: disable=no-member

import abc
import collections
import turtle as t

class Pos:
    """点的位置的定义"""

    def __init__(self, x, y):
        #pylint: disable=invalid-name
        self.x = x
        self.y = y

    def __add__(self, other):
        try:
            return self.__class__(self.x + other.x, self.y + other.y)
        except AttributeError:
            return NotImplemented

    def __iter__(self):
        yield self.x
        yield self.y

    def __neg__(self):
        return self.__class__(-self.x, -self.y)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash(self.x) ^ hash(self.y)

    def __sub__(self, other):
        return self + (-other)

    def __mul__(self, num):
        return self.__class__(self.x * num, self.y * num)

    def __rmul__(self, num):
        return self * num

    def __repr__(self):
        cls_name = self.__class__.__name__
        return "{}(x={}, y={})".format(cls_name, self.x, self.y)

class Direction:
    """方向"""

    NONE = 'none'
    UP = 'up'
    DOWN = 'down'
    LEFT = 'left'
    RIGHT = 'right'

    DELTA = {
        NONE: Pos(0, 0),
        UP: Pos(0, 1),
        DOWN: Pos(0, -1),
        LEFT: Pos(-1, 0),
        RIGHT: Pos(1, 0),
    }

    @classmethod
    def get_direct_delta(cls, direct):
        """获得指定方向的偏移量"""
        return cls.DELTA[direct]


# pylint: disable=undefined-variable
class Drawable(metaclass=abc.ABCMeta):
    """所有界面上可以显示的对象的抽象基类"""

    RED = 'red'
    GREEN = 'green'
    BLUE = 'blue'
    BLACK = 'black'
    WHITE = 'white'

    # four point of a object(rectangle)
    HitBox = collections.namedtuple('HitBox', 'bot_l top_r')

    @abc.abstractmethod
    def draw(self):
        """在界面上画出自身的接口"""
        pass

    @abc.abstractmethod
    def get_hitbox(self):
        """获得自身的碰撞范围"""
        pass

    def __init__(self, color):
        self.color = color

    def erase(self):
        """在界面上清除对象"""
        old_color, self.color = self.color, Drawable.WHITE
        self.draw()
        self.color = old_color

    @classmethod
    def check_hitbox(cls, obj_a, obj_b):
        """检测两个物体的碰撞情况"""
        if (not issubclass(obj_a.__class__, cls)
                or not issubclass(obj_b.__class__, cls)):
            raise TypeError('obj must be subclass of {}'.format(cls.__name__))

        is_collision = False
        for a_box in obj_a.get_hitbox():
            for b_box in obj_b.get_hitbox():
                if cls.check_rectangle(a_box, b_box):
                    is_collision = True
                    break
            if is_collision:
                break

        return is_collision

    @staticmethod
    def check_rectangle(rec_a, rec_b):
        """检查两个举行是否碰撞"""
        x_collision = True
        if (rec_a.top_r.x <= rec_b.bot_l.x
                or rec_a.bot_l.x >= rec_b.top_r.x):
            x_collision = False
        y_collision = True
        if (rec_a.bot_l.y >= rec_b.top_r.y
                or rec_a.top_r.y <= rec_b.bot_l.y):
            y_collision = False

        is_collision = False
        if x_collision and y_collision:
            is_collision = True

        return is_collision

class StateMinix():
    """描述一个对象的状态"""

    LIVE_FOREVER = -1

    def __init__(self):
        self.ttl = self.LIVE_FOREVER
        self.direction = Direction.NONE
        self.speed = 0

    def alive(self):
        """对象是否存在"""
        return self.ttl != 0


class Apple(Drawable, StateMinix):
    """贪吃蛇的食物"""

    def __init__(self, pos, *, size=30, color=Drawable.RED):
        super().__init__(color)
        StateMinix.__init__(self)
        self._pos = pos
        self._size = size

    def draw(self):
        t.up()
        t.goto(*self._pos)
        t.dot(self._size, self.color)

    def get_hitbox(self):
        top_r = self._pos + Pos(self._size/2, self._size/2)
        bot_l = self._pos - Pos(self._size/2, self._size/2)
        return [Drawable.HitBox(bot_l, top_r)]

    @abc.abstractmethod
    def eat_by(self, snake):
        """吃掉苹果产生的变化"""
        pass

    def __repr__(self):
        cls_name = self.__class__.__name__
        return "{}(pos={}, size={}, color={})".format(cls_name, self._pos,
                                                      self._size, self.color)


class GrowApple(Apple):
    """吃掉后可以让身体长一节"""

    def eat_by(self, snake):
        snake.grow()


class Snake(Drawable, StateMinix):
    """一只贪吃蛇, 喜欢吃苹果"""

    def __init__(self, pos=Pos(0, 0), *, size=30, color=Drawable.GREEN):
        super().__init__(color)
        StateMinix.__init__(self)
        self.size = size
        self.body = [pos]
        self._prev_tail = None

    def draw(self):
        for chunk in self.body:
            self._draw_chunk(chunk)

    def get_hitbox(self):
        hitbox = []
        for chunk in self.body:
            hitbox.append(self._get_chunk_hitbox(chunk))
        return hitbox

    def to_up(self):
        """向上移动"""
        if self.direction not in (Direction.UP, Direction.DOWN):
            self.direction = Direction.UP

    def to_down(self):
        """向下移动"""
        if self.direction not in (Direction.UP, Direction.DOWN):
            self.direction = Direction.DOWN

    def to_left(self):
        """向左移动"""
        if self.direction not in (Direction.LEFT, Direction.RIGHT):
            self.direction = Direction.LEFT

    def to_right(self):
        """向右移动"""
        if self.direction not in (Direction.LEFT, Direction.RIGHT):
            self.direction = Direction.RIGHT

    def move(self):
        """按照既定方向移动贪吃蛇"""
        self._prev_tail = self.body[-1]
        for index in range(len(self.body) - 1, 0, -1):
            self.body[index] = self.body[index - 1]
        new_pos = self.size * Direction.get_direct_delta(self.direction)
        self.body[0] += new_pos

    def grow(self):
        """生长一节"""
        self.body.append(self._prev_tail)

    def _draw_chunk(self, pos):
        """绘出一节躯干"""
        t.up()
        t.goto(*pos)
        t.dot(self.size, self.color)

    def _get_chunk_hitbox(self, pos):
        """获得一节躯干的hitbox"""
        top_r = pos + Pos(self.size/2, self.size/2)
        bot_l = pos - Pos(self.size/2, self.size/2)
        return Drawable.HitBox(bot_l, top_r)

    def __repr__(self):
        cls_name = self.__class__.__name__
        return "{}(pos={}, size={}, color={})".format(cls_name, self.body[0],
                                                      self.size, self.color)

class Wall(Drawable):
    """世界里的墙"""

    def __init__(self, bot_l, top_r):
        super().__init__(Drawable.BLACK)
        self._bot_l = bot_l
        self._top_r = top_r
        self._bot_r = Pos(top_r.x, bot_l.y)
        self._top_l = Pos(bot_l.x, top_r.y)

    def draw(self):
        t.up()
        t.fillcolor(self.color)
        t.goto(*self._bot_l)
        t.begin_fill()
        t.goto(*self._bot_r)
        t.goto(*self._top_r)
        t.goto(*self._top_l)
        t.end_fill()

    def get_hitbox(self):
        return [Drawable.HitBox(self._bot_l, self._top_r)]

    def __repr__(self):
        cls_name = self.__class__.__name__
        return "{}(bot_l={}, top_r={})".format(cls_name, self._bot_l,
                                               self._top_r)
