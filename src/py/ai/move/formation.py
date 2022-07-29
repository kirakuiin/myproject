"""阵型
"""
import enum
import operator
import collections

import math2d
from . import defines


class CharacterInterface(object):
    """角色"""
    def set_target(self, target: defines.Location):
        """设置角色目标定位

        @param target:
        @return:
        """


class CharacterTypeInterface(object):
    """角色类型"""
    def set_char_type(self, type: enum.IntEnum):
        """设置角色类型

        @param type:
        @return:
        """

    def get_char_type(self) -> enum.IntEnum:
        """返回角色类型

        @return:
        """


class PatternInterface(object):
    """编队模式"""
    def get_drift_offset(self, assignments: list) -> defines.Location:
        """获得偏移量

        @return:
        """
        position = math2d.vector()
        total_num = len(assignments)
        for assignment in assignments:
            location = self.get_slot_location(total_num, assignment.slot_num)
            position += location.position
        position = position/total_num
        return defines.Location(position)

    def get_slot_location(self, total_num: int, slot_num: int) -> defines.Location:
        """根据槽位编号获得位置

        @param total_num: 总槽位数量
        @param slot_num: 当前槽位编号
        @return: (位置, 角度)
        """

    def is_support_slot(self, slot_num: int) -> bool:
        """是否支持指定的槽位编号

        @param slot_num:
        @return:
        """

    def get_slot_cost(self, slot_num: int, character: CharacterTypeInterface) -> int:
        """获得指定角色在指定槽位的成本

        @param character:
        @param slot_num:
        @return:
        """


class SlotAssignment(object):
    """槽位分配"""
    def __init__(self, character: CharacterInterface, slot_num: int):
        self.character = character  # 角色
        self.slot_num = slot_num  # 槽位编号


class StaticCharacter(CharacterInterface, CharacterTypeInterface, defines.StaticObj):
    """静态角色"""
    def __init__(self):
        super(StaticCharacter, self).__init__(math2d.vector())
        self._type = None  # 角色类型

    def __repr__(self):
        return '{}(type={})'.format(self.__class__.__name__, self._type and self._type.name or None)

    def set_target(self, target: defines.Location):
        self.set_pos_vec(target.position)
        self.set_rotate(target.orientation)

    def set_char_type(self, type):
        self._type = type

    def get_char_type(self) -> int:
        return self._type

    def update(self, dt):
        pass


class FormationManager(CharacterInterface):
    """槽位管理器"""
    def __init__(self, pattern: PatternInterface):
        self._pattern = pattern  # 编队模式
        self._slots = []  # 槽位
        self._drift_offset = defines.Location()  # 质心偏移量
        self._anchor_point = defines.Location()  # 锚点

    def update_slots(self):
        for slot in self._slots:
            location = self._pattern.get_slot_location(len(self._slots), slot.slot_num)
            location.position -= self._drift_offset.position
            location.orientation -= self._drift_offset.orientation
            location.position = self._anchor_point.position+math2d.rotate(location.position, self._anchor_point.orientation)
            location.orientation += self._anchor_point.orientation
            slot.character.set_target(location)

    def add_character(self, character):
        total_slot_num = len(self._slots)
        if self._pattern.is_support_slot(total_slot_num):
            self._slots.append(SlotAssignment(character, total_slot_num))
            self._update_slot_assignments()

    def _update_slot_assignments(self):
        for idx, slot in enumerate(self._slots):
            slot.slot_num = idx
        self._drift_offset = self._pattern.get_drift_offset(self._slots)

    def remove_character(self, character):
        idx = self._find_idx_of_character(character)
        if idx is not None:
            self._slots.pop(idx)
            self._update_slot_assignments()

    def _find_idx_of_character(self, character):
        for slot in self._slots:
            if slot.character is character:
                return slot.slot_num
        else:
            return None

    def set_target(self, target: defines.Location):
        self._anchor_point = target
        self.update_slots()

    def get_anchor(self) -> defines.Location:
        return self._anchor_point


class DefenceCirclePattern(PatternInterface):
    """环形防御编队

    可支持任意数量角色, 角色面朝外紧贴环形排列
    """
    def __init__(self, character_radius):
        self._char_radius = character_radius  # 角色半径

    def is_support_slot(self, slot_num: int) -> bool:
        return True

    def get_slot_location(self, total_slot_num: int, slot_num: int) -> defines.Location:
        if total_slot_num == 1:
            return defines.Location()
        angle = slot_num/total_slot_num*(2*math2d.pi)
        radius = self._char_radius/math2d.sin(math2d.pi/total_slot_num)
        position = math2d.position(radius*math2d.cos(angle), radius*math2d.sin(angle))
        return defines.Location(position, math2d.degrees(angle))


class TrianglePattern(PatternInterface):
    """三角编队

    最多支持3个人
    """
    def __init__(self, width, height):
        self._width = width  # 编队宽度
        self._height = height  # 编队高度

    def is_support_slot(self, slot_num: int) -> bool:
        return slot_num < 3

    def get_slot_location(self, total_slot_num: int, slot_num: int) -> defines.Location:
        location = defines.Location()
        if slot_num == 0:
            location.position = math2d.position(self._height, 0)
        elif slot_num == 1:
            location.position = math2d.position(0, self._width/2)
        else:
            location.position = math2d.position(0, -self._width/2)
        return location


class TDHEnum(enum.IntEnum):
    """战法牧体系类型"""
    TANK = 1  # 坦克
    DPS = 2  # 输出
    HEALER = 3  # 治疗


class AdventurePattern(PatternInterface):
    """冒险阵型"""
    def __init__(self, distance):
        self._distance = distance  # 任务间隔
        self._cost_map = {
            0: {TDHEnum.TANK: 0, TDHEnum.DPS: 99, TDHEnum.HEALER: 999},
            1: {TDHEnum.TANK: 0, TDHEnum.DPS: 99, TDHEnum.HEALER: AssignFormationManager.COST_MAX},
            2: {TDHEnum.TANK: 99, TDHEnum.DPS: 49, TDHEnum.HEALER: 0},
            3: {TDHEnum.TANK: 99, TDHEnum.DPS: 0, TDHEnum.HEALER: 49},
        }

    def is_support_slot(self, slot_num: int) -> bool:
        return slot_num < 4

    def get_slot_cost(self, slot_num: int, character: CharacterTypeInterface) -> int:
        return self._cost_map[slot_num][character.get_char_type()]

    def get_slot_location(self, total_slot_num: int, slot_num: int) -> defines.Location:
        location = defines.Location()
        if slot_num == 0:
            location.position = math2d.position(self._distance, self._distance/2)
        elif slot_num == 1:
            location.position = math2d.position(self._distance, -self._distance/2)
        elif slot_num == 2:
            location.position = math2d.position()
        else:
            location.position = math2d.position(-self._distance)
        return location


class AssignFormationManager(FormationManager):
    """根据成本进行分配的阵型管理器"""
    COST_MAX = 1000
    CostIdx = collections.namedtuple('CostIdx', 'cost, idx')
    CharacterSlots = collections.namedtuple('CharacterSlot', 'character, assignment_ease, cost_idx_list')

    def __init__(self, pattern):
        super().__init__(pattern)

    def _update_slot_assignments(self):
        """根据计算的难易度重新分配槽位

        @return:
        """
        datum_list = self._get_character_ease_list()  # 计算槽位分配难度值
        occupy_slots = set()
        self._slots = []
        for datum in datum_list:
            for cost_idx in datum.cost_idx_list:
                if cost_idx.idx not in occupy_slots:
                    occupy_slots.add(cost_idx.idx)
                    self._slots.append(SlotAssignment(datum.character, cost_idx.idx))
                    break
            else:
                raise math2d.NoSolutionException('角色 {} 无法分配'.format(datum.character))
        self._drift_offset = self._pattern.get_drift_offset(self._slots)

    def _get_character_ease_list(self):
        datum_list = []
        for slot in self._slots:
            cost_list = []
            assignment_ease = 0
            for slot_num in range(len(self._slots)):
                cost_idx = self.CostIdx(self._pattern.get_slot_cost(slot_num, slot.character), slot_num)
                if cost_idx.cost < self.COST_MAX:
                    cost_list.append(cost_idx)
                    assignment_ease += 1 / (1 + cost_idx.cost)
            cost_list.sort(key=operator.attrgetter('cost'))
            datum_list.append(self.CharacterSlots(slot.character, assignment_ease, cost_list))
        datum_list.sort(key=operator.attrgetter('assignment_ease'))
        return datum_list

