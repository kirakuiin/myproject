"""贪吃蛇

提高分数
控制蛇吃掉苹果会增加长度并提高分数
撞到墙壁或者自身会导致游戏结束

"""

# pylint: disable=no-member
import random
import sys
import turtle as t

import snake as s

class World:
    """管理贪吃蛇中所有元素的生成以及消除"""

    def __init__(self):
        self.walls = {}
        self.snakes = {}
        self.apples = {}
        self._drawables = [self.walls, self.snakes, self.apples]

    def set_wall(self, wall_arr):
        """设置墙壁的位置"""
        for wall in wall_arr:
            a_wall = s.Wall(s.Pos(*wall['bot_l']), s.Pos(*wall['top_r']))
            self.walls[a_wall] = a_wall.get_hitbox()

    def draw_walls(self):
        """画出墙壁"""
        for wall in self.walls:
            wall.draw()

    def gene_apple(self, bot_l, top_r, apple_cls=s.GrowApple, *, size=30):
        """生成苹果并返回其实例"""
        while True:
            pos_x = random.randint(bot_l.x + size//2, top_r.y - size//2)
            pos_y = random.randint(bot_l.y + size//2, top_r.y - size//2)
            apple = apple_cls(pos=s.Pos(pos_x, pos_y), size=size)

            can_gene = True
            for drawable in self._drawables:
                for obj in drawable:
                    if s.Drawable.check_hitbox(apple, obj):
                        can_gene = False
                        break
            if can_gene:
                self.apples[apple] = apple.get_hitbox()
                break
        return apple

    def get_apple_num(self):
        """获得场地内苹果的数量"""
        return len(self.apples)

    def gene_snake(self, pos, size=20):
        """生成一只蛇并返回其实例"""
        #pylint: disable=not-callable
        snake = s.Snake(pos, size=size)
        self.snakes[snake] = snake.get_hitbox()
        return snake

    def erase(self):
        """清除蛇和苹果"""
        for snake in self.snakes:
            snake.erase()
        for apple in self.apples:
            apple.erase()
        t.update()

    def draw(self):
        """画出蛇和苹果"""
        for apple in self.apples:
            apple.draw()
        for snake in self.snakes:
            snake.draw()
        t.update()

    def redraw(self):
        """重新绘制"""
        t.clear()
        self.draw_walls()
        self.draw()


class EventExecutor:
    """执行所有操作以及界面交互"""

    WALLS = [
        {'bot_l': (-200, -200), 'top_r': (200, -190)},
        {'bot_l': (-200, -200), 'top_r': (-190, 200)},
        {'bot_l': (-200, 190), 'top_r': (200, 200)},
        {'bot_l': (190, -200), 'top_r': (200, 190)},
    ]

    APPLE_BL = s.Pos(-190, -190)
    APPLE_TR = s.Pos(190, 190)

    RUNNING = 0
    PAUSE = 1
    STOP = 2
    QUIT = 3

    def __init__(self):
        self._state = True
        self._world = World()
        self.players = {}

    def add_player(self, controller):
        """添加一个蛇单位"""
        pos = s.Pos(random.randint(-100, 100), random.randint(-100, 100))
        snake = self._world.gene_snake(pos)
        controller.world = self._world
        controller.snake = snake
        self.players[controller] = snake
        controller.setup()

    def start(self):
        """开始游戏"""
        t.ht()
        t.tracer(False)
        t.listen()
        t.onkey(self._continue, 'space')
        t.onkey(self._quit, 'q')
        t.onkey(self._pause, 'p')

        self._world.set_wall(self.WALLS)
        self._world.draw_walls()

        self._state = self.RUNNING
        self._run()
        t.mainloop()

    def _continue(self):
        """继续游戏"""
        self._state = self.RUNNING

    def _quit(self):
        """停止游戏"""
        self._state = self.QUIT

    def _pause(self):
        """暂停游戏"""
        self._state = self.PAUSE

    def _run(self):
        """执行事件循环"""

        if self._state == self.RUNNING:
            self._game_exec()
        elif self._state == self.PAUSE:
            pass
        elif self._state == self.STOP:
            self._game_stop()
        else:
            sys.exit()
        t.ontimer(self._run, 200)

    def _game_exec(self):
        """游戏的具体执行进程"""
        self._world.erase()
        if self._world.get_apple_num() == 0:
            self._world.gene_apple(self.APPLE_BL, self.APPLE_TR)

        for player, snake in self.players.items():
            player.control()
            snake.move()
            self.hit_detect(player)

        self._world.draw()

    def _game_stop(self):
        """蛇死亡场景"""
        deads = []
        for player in self.players:
            if player.alive is not True:
                deads.append(player)
                print(player.score)
        for dead in deads:
            snake = self.players[dead]
            self._world.snakes.pop(snake)
            self.players.pop(dead)

        self._world.redraw()
        self._state = self.RUNNING

    def hit_detect(self, player):
        """检查蛇的碰撞情况"""
        snake = self.players[player]
        for wall in self._world.walls:
            if s.Drawable.check_hitbox(snake, wall):
                self._state = self.STOP
                player.alive = False
                break
        for apple in self._world.apples:
            if s.Drawable.check_hitbox(snake, apple):
                player.score += 100
                self._world.apples.pop(apple)
                apple.eat_by(snake)
                break
        hitbox = snake.get_hitbox()
        head = hitbox[0]
        for a_snake in self._world.snakes:
            if snake is a_snake:
                continue
            else:
                for body in a_snake.get_hitbox():
                    if s.Drawable.check_rectangle(head, body):
                        self._state = self.STOP
                        player.alive = False
                        break
        for body in hitbox[1:]:
            if s.Drawable.check_rectangle(head, body):
                self._state = self.STOP
                player.alive = False
                break
