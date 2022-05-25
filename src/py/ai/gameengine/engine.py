"""游戏引擎入口"""

import pygame
import collections
import time

from . import defines
from . import global_vars
from . import uiobject


HandleInfo = collections.defaultdict(list)  # 事件回调容器


def init_engine():
    """初始化引擎

    @return:
    """
    pygame.init()
    global_vars.screen = pygame.display.set_mode((defines.SCREEN_WIDTH, defines.SCREEN_HEIGHT))
    global_vars.scene = uiobject.UIObject()
    pygame.display.set_caption(defines.SCREEN_TITLE)
    clean_screen()


def clean_screen():
    """清除屏幕内容
    """
    global_vars.screen.fill((100, 100, 100))


def quit_engine():
    """关闭引擎
    @return:
    """
    global_vars.is_running = False


def start_engine():
    """启动引擎

    @return:
    """
    global_vars.start_time = time.time()
    global_vars.is_running = True
    clock = pygame.time.Clock()
    render()
    while global_vars.is_running:
        clock.tick(defines.FPS)
        global_vars.run_time = time.time() - global_vars.start_time
        handle_event()
        update(global_vars.scene, clock.get_time()/defines.MS_PER_SECOND)
        render()
    pygame.quit()


def handle_event():
    """处理事件

    @return:
    """
    for event in pygame.event.get():
        if event.type in HandleInfo:
            for cb in HandleInfo[event.type]:
                cb(event)
        if event.type == pygame.QUIT:
            quit_engine()


def render():
    """渲染画面

    @return:
    """
    if not global_vars.is_show_track:
        clean_screen()
    root = global_vars.scene
    root.render(uiobject.Transform(), False)
    pygame.display.update()


def update(root, dt):
    """驱动对象更新

    @param root:
    @param dt:
    @return:
    """
    root.update(dt)
    for child in root.get_children():
        update(child, dt)
