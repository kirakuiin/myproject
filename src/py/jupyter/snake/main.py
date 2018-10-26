"""贪吃蛇

提高分数
控制蛇吃掉苹果会增加长度并提高分数
撞到墙壁或者自身会导致游戏结束

"""

from world import EventExecutor
from control import Player, AI

if __name__ == '__main__':
    # pylint: disable=invalid-name
    game = EventExecutor()
    player = Player()
    ai = AI()
    game.add_player(player)
    game.add_player(ai)
    game.start()
