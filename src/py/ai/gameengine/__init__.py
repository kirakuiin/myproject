"""引擎封装包
"""

from . import global_vars
from . import engine


def init():
    """初始化引擎

    @return:
    """
    engine.init_engine()


def start():
    """启动引擎

    @return:
    """
    engine.start_engine()


def quit():
    """退出引擎

    @return:
    """
    engine.quit_engine()


def get_screen():
    """获得屏幕引用

    @return:
    """
    return global_vars.screen


def get_scene():
    """获得场景引用

    @return:
    """
    return global_vars.scene


def set_show_track(is_show):
    """是指是否绘制运动轨迹

    @param is_show:
    @return:
    """
    global_vars.is_show_track = is_show


def register_handle(event_type, handle_func):
    """注册事件处理函数

    @param event_type: 事件类型
    @param handle_func: 处理函数
    @return:
    """
    engine.HandleInfo[event_type].append(handle_func)