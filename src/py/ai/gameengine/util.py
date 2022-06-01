"""实用函数"""

import pygame

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
