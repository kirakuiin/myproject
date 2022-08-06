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


def convert_to_world_space(game_obj, position: math2d.ndarray) -> math2d.ndarray:
    """将局部坐标以目标节点为父节点转化为世界坐标

    @param game_obj:
    @param position:
    @return:
    """
    return convert_to_world_transform(game_obj, math2d.Transform(position)).pos


def convert_to_world_transform(game_obj, transform):
    """将局部变换以目标节点为父节点转化为世界变换

    @param game_obj:
    @param transform:
    @return:
    """
    if game_obj is None:
        return transform
    if game_obj.get_parent() is None:
        return game_obj.transform.transform
    obj_world_transform = convert_to_world_transform(game_obj.get_parent(), game_obj.transform.transform)
    return transform.combine(obj_world_transform)


def convert_to_local_space(game_obj, position: math2d.ndarray) -> math2d.ndarray:
    """将世界坐标转化为以目标节点为父节点的局部坐标

    @param game_obj:
    @param position:
    @return:
    """
    transform = convert_to_world_transform(game_obj.get_parent(), game_obj.transform.transform)
    invert_matrix =math2d.Transform.get_invert_matrix(transform.pos, transform.rotate, transform.scales)
    return invert_matrix@position