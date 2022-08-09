"""游戏引擎入口"""

import sys
import collections
import pygame
import time

from . import defines
from . import global_vars
from . import uiobject
from . import camera
from . import event


def init_engine():
    """初始化引擎

    @return:
    """
    pygame.init()
    pygame.display.set_caption(defines.SCREEN_TITLE)
    global_vars.screen = pygame.display.set_mode((defines.SCREEN_WIDTH, defines.SCREEN_HEIGHT))
    global_vars.scene = uiobject.Scene()
    global_vars.camera_mgr = camera.CameraMgr()
    global_vars.event_mgr = event.EventMgr()
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
    global_vars.event_mgr.register(pygame.QUIT, lambda evt: quit_engine())
    clock = pygame.time.Clock()
    render()
    while global_vars.is_running:
        delta_time = clock.get_time()/defines.MS_PER_SECOND*global_vars.speed_times
        global_vars.game_run_time += delta_time
        global_vars.real_run_time = time.time() - global_vars.start_time
        global_vars.event_mgr.propagate()
        update(global_vars.scene, delta_time)
        clear_expired_obj()
        render()
        clock.tick(defines.FPS)
    pygame.quit()


def update(root, dt):
    """驱动对象更新

    @param root:
    @param dt:
    @return:
    """
    root.update(dt)
    for child in root.transform.children:
        update(child, dt)


def clear_expired_obj():
    """清理过期对象

    @return:
    """
    queue = collections.deque(global_vars.scene.transform.children)
    while queue:
        cur = queue.popleft()
        if sys.getrefcount(cur) == 3:
            cur.get_parent().remove_child(cur)
        else:
            queue.extend(cur.transform.children)


def render():
    """渲染画面

    @return:
    """
    clean_screen()
    global_vars.camera_mgr.render_all_camera()
    pygame.display.update()