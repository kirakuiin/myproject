"""实用函数"""

import pygame
import math2d

def get_window_size() -> tuple:
    """返回屏幕大小

    @return: 屏幕大小
    """
    return pygame.display.get_window_size()


def get_window_width() -> int:
    """返回屏幕宽度

    @return:
    """
    return get_window_size()[0]


def get_window_height() -> int:
    """返回屏幕高度

    @return:
    """
    return get_window_size()[1]


def get_window_coord(pos: math2d.ndarray) -> list:
    """将坐标转换为窗口坐标

    @param pos:
    @return: [x, y]
    """
    window_pos = math2d.position(pos[0], get_window_height()-pos[1])
    return [window_pos[0], window_pos[1]]

