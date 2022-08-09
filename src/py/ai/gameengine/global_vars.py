"""全局变量
"""

start_time = 0  # 游戏开始时间
real_run_time = 0   # 游戏运行时间(现实世界)
game_run_time = 0   # 游戏运行时间(游戏世界)
speed_times = 1.0  # 游戏加速度
is_running = False  # 是否正在运行

screen = None  # 屏幕对象
scene = None  # 场景节点
camera_mgr = None  # 相机管理器
event_mgr = None  # 事件管理器