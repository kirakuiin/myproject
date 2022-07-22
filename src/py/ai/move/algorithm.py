"""移动算法"""

import math2d
from .defines import KinematicInterface as Kinematic


def get_seek_acc(src_pos: math2d.ndarray, des_pos: math2d.ndarray, acc: float) -> math2d.ndarray:
    """根据对象位置寻找加速度

    @param src_pos: 源位置
    @param des_pos: 目标位置
    @param acc: 加速度
    """
    direction = math2d.normalize(des_pos - src_pos)
    acceleration = direction * acc
    return acceleration


def get_align_acc(src_obj: Kinematic, des_rot: float, brake_radius=30, near_time=0.25):
    """计算对齐角加速度

    @param src_obj: 源对象
    @param des_rot: 目标角度
    @param brake_radius: 减速半径
    @param near_time: 接近时间
    @return:
    """
    angular = get_speed_by_distance(abs(math2d.angle_delta(src_obj.rotation(), des_rot)), src_obj.max_angular(),
                                    brake_radius)
    direction = math2d.normalize(math2d.angle_delta(src_obj.rotation(), des_rot))
    return get_match_acc(src_obj.angular(), angular * direction, src_obj.max_angular_acc(), near_time)


def get_face_acc(src_obj: Kinematic, des_pos: math2d.ndarray, brake_radius=30, near_time=0.25):
    """计算朝向角加速度

    @param src_obj: 源对象
    @param des_pos: 目标位置
    @param brake_radius: 减速半径
    @param near_time: 接近时间
    @return:
    """
    des_rot = math2d.as_degrees(des_pos - src_obj.position())
    return get_align_acc(src_obj, des_rot, brake_radius, near_time)


def get_speed_by_distance(distance: float, max_speed: float, brake_radius: float):
    """根据距离来计算合适的速度

    距离越近速度越低, 当距离为0时速度应为0
    @param distance: 距离
    @param max_speed: 最大速度
    @param brake_radius: 减速半径
    @return:
    """
    if distance > brake_radius:
        return max_speed
    else:
        return max_speed * distance / brake_radius


def get_match_acc(src_speed, des_speed, max_acc=100.0, near_time=0.25) -> math2d.ndarray:
    """计算匹配指定角速度所需要的加速度

    @param src_speed: 源速度
    @param des_speed: 目标速度
    @param max_acc: 最大加速度
    @param near_time: 匹配时间
    @return: 加速度
    """
    acc = (des_speed - src_speed) / near_time
    if math2d.norm(acc) > max_acc:
        acc = math2d.normalize(acc) * max_acc
    return acc


def is_close_enough(src_pos: math2d.ndarray, des_pos: math2d.ndarray, near_threshold=5) -> bool:
    """判断两个对象是否足够接近

    @param src_pos: 来源位置
    @param des_pos: 目标位置
    @param near_threshold: 接近阈值
    @return:
    """
    return math2d.distance(src_pos, des_pos) < near_threshold


def get_near_time(src_pos: math2d.ndarray, src_vec: math2d.ndarray, des_pos: math2d.ndarray,
                  des_vec: math2d.ndarray) -> float:
    """判断两个对象到达最近距离所需要的时间

    如果返回值小于0则代表永远不会接近
    @param src_pos: 来源位置
    @param src_vec: 来源速度
    @param des_pos: 目标位置
    @param des_vec: 目标速度
    @return: float
    """
    dp = des_pos - src_pos
    dv = src_vec - des_vec
    length = math2d.norm(dv)
    if length <= 0:
        return 0 if math2d.norm(dp) == 0 else -1
    return math2d.dot(dp, dv) / length ** 2


def get_firing_solution(start: math2d.ndarray, end: math2d.ndarray, speed: float, gravity: math2d.ndarray, is_shortest=True):
    """求解从以固定速度值从一点到另一点的对象所需要的初速度方向(只受重力影响)

    @param start: 起点
    @param end: 终点
    @param speed: 速度值
    @param gravity: 重力
    @param is_shortest: 是否选最短时间的速度
    @return: 单位方向向量
    """
    delta = end - start
    a = math2d.norm(gravity)**2
    b = -4*(math2d.dot(gravity, delta)+speed**2)
    c = 4*(math2d.norm(delta)**2)
    root = b**2-4*a*c
    if root < 0:  # 无实数解
        raise math2d.NoSolutionException('无实数解')
    else:
        times = [t for t in [math2d.sqrt((-b+math2d.sqrt(root))/(2*a)), math2d.sqrt((-b-math2d.sqrt(root))/(2*a))] if t > 0]
        if not times:
            raise math2d.NoSolutionException('有效时间均为负数')
        else:
            faster_time = min(times) if is_shortest else max(times)
            return (delta*2-gravity*(faster_time**2))/(2*speed*faster_time)


def get_firing_solution_with_resist(start: math2d.ndarray, end: math2d.ndarray, speed: float, gravity: math2d.ndarray,
                                    simulate_func, margin: float = 1):
    """求解从以固定速度值从一点到另一点的对象所需要的初速度方向(受重力和阻力影响)

    @param start: 起点
    @param end: 终点
    @param speed: 速度值
    @param gravity: 重力
    @param simulate_func: 模拟运行结果函数
    @param margin: 余量
    @return: 单位方向向量
    """
    max_bound = math2d.as_vector(45)
    min_bound = math2d.as_vector(-45)

    def is_done(distance):
        return abs(distance) < margin

    def get_direction(angle):
        base = end - start
        vec = math2d.as_vector(angle)
        return vec if base[0] >= 0 else vec*math2d.vector(-1, 1)

    def get_angle(direction):
        return math2d.degrees(math2d.arcsin(direction[1]/math2d.norm(direction)))

    def check_angle(angle):
        direction = get_direction(angle)
        distance = simulate_func(direction)
        return direction, distance

    def set_bound(angle, distance):
        nonlocal max_bound, min_bound
        if distance > 0:
            max_bound = angle
            min_bound = -45
            direction, distance = check_angle(min_bound)
        else:
            max_bound = 45
            min_bound = angle
            direction, distance = check_angle(max_bound)
            if distance < 0:
                raise math2d.NoSolutionException('无法到达目标')
        return direction, distance

    def binary_search(direction, distance):
        nonlocal max_bound, min_bound
        while not is_done(distance):
            angle = (max_bound-min_bound)/2+min_bound
            direction, distance = check_angle(angle)
            if distance < 0:
                min_bound = angle
            else:
                max_bound = angle
        return direction

    direction = get_firing_solution(start, end, speed, gravity)
    distance = simulate_func(direction)
    if is_done(distance):
        return direction
    direction, distance = set_bound(get_angle(direction), distance)
    return binary_search(direction, distance)


def get_jump_x_speed(jump_point, jump_speed, gravity, max_speed):
    """获取跳跃所需要的x轴速度

    @param jump_point: 跳跃点数据
    @param jump_speed: 跳跃速度(y轴)
    @param gravity: 重力(y轴)
    @param max_speed: 对象的最大速度
    @return: x轴速度
    """
    def calc_jump_time():
        jump_vec = jump_point.as_vector()
        sqrt_term = 2*gravity*jump_vec[1]+jump_speed**2
        if sqrt_term < 0:
            raise math2d.NoSolutionException('无实数解')
        sqrt_term = math2d.sqrt(sqrt_term)
        return (-jump_speed+sqrt_term)/gravity, (-jump_speed-sqrt_term)/gravity

    def check_jump_time(time):
        jump_vec = jump_point.as_vector()
        vx = jump_vec[0]/time
        if vx**2+jump_speed**2 > max_speed**2:
            return False
        else:
            return True

    short, long = calc_jump_time()
    if check_jump_time(short):
        return jump_point.as_vector()[0]/short
    elif check_jump_time(long):
        return jump_point.as_vector()[0]/long
    else:
        raise math2d.NoSolutionException('无法到达目标')
