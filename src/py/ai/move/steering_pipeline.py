"""转向管道
"""

import math2d
from . import defines
from . import steering_behavior


class Goal(object):
    """到达目标所需要的通道参数
    """
    def __init__(self):
        self.position = math2d.position()  # 位置
        self.rotate = 0.0  # 旋转
        self.velocity = math2d.vector()  # 速度
        self.angular = 0.0  # 角速度

    def _has_value(self, channel):
        return math2d.norm(channel) > defines.EPSILON

    def update(self, other):
        """更新目标

        @param other:
        @return:
        """
        if self._has_value(other.position):
            self.position = other.position
        if self._has_value(other.rotate):
            self.rotate = other.rotate
        if self._has_value(other.velocity):
            self.velocity = other.velocity
        if self._has_value(other.angular):
            self.angular = other.angular


class TargeterInterface(object):
    """目标器接口

    返回一个目标
    """
    def get_goal(self) -> Goal:
        pass


class DecomposerInterface(object):
    """分解器接口

    将目标分解为子目标
    """
    def decompose(self, src_obj: defines.KinematicInterface, goal: Goal) -> Goal:
        pass


class ConstrainInterface(object):
    """约束器接口

    判断规划路径是否有效
    """
    def is_violate(self, path) -> bool:
        pass

    def suggest(self, path) -> Goal:
        """返回一个推荐目标

        @param path:
        @return:
        """
        pass


class ActuatorInterface(object):
    """执行器接口

    返回实际的转向操作
    """
    def get_path(self, src_obj: defines.KinematicInterface, goal: Goal):
        pass

    def get_steering_output(self, src_obj: defines.KinematicInterface, goal: Goal) -> defines.AccOutput:
        pass


class SteeringPipeline(object):
    """转向管道
    """
    TRY_NUMBER = 10  # 约束尝试执行次数
    def __init__(self, src_obj: defines.KinematicInterface):
        self._src_obj = src_obj  # 运行目标
        self._targeter_list = []  # 目标器列表
        self._decomposer_list = []  # 分解器列表
        self._constraint_list = []  # 约束器列表
        self._actuator = None  # 执行器

        self._deadlock_behavior = None  # 死锁执行行为

    def add_targeter(self, targeter: TargeterInterface):
        self._targeter_list.append(targeter)

    def add_decomposer(self, decomposer: DecomposerInterface):
        self._decomposer_list.append(decomposer)

    def add_constraint(self, constraint: ConstrainInterface):
        self._constraint_list.append(constraint)

    def set_actuator(self, actuator: ActuatorInterface):
        self._actuator = actuator

    def set_deadlock_behavior(self, behavior: steering_behavior.SteeringBehaviorInterface):
        self._deadlock_behavior = behavior

    def get_steering_output(self) -> defines.AccOutput:
        goal = self._find_goal()
        goal = self._decompose_goal(goal)
        return self._get_actuator_output(goal)

    def _find_goal(self):
        goal = Goal()
        for targeter in self._targeter_list:
            goal.update(targeter.get_goal(goal))
        return goal

    def _decompose_goal(self, goal):
        for decomposer in self._decomposer_list:
            goal = decomposer.decompose(self._src_obj, goal)
        return goal

    def _get_actuator_output(self, goal):
        for i in range(self.TRY_NUMBER):
            path = self._actuator.get_path(self._src_obj, goal)
            for constraint in self._constraint_list:
                if constraint.is_violate(path):
                    goal = constraint.suggest(path)
                    break
            else:
                return self._actuator.get_steering_output(self._src_obj, goal)
        return self._deadlock_behavior.get_steering_output()


