"""阵型
"""

import math2d
from . import defines


class CharacterInterface(object):
    """角色"""
    def set_target(self, target: defines.Location):
        """设置角色目标定位

        @param target:
        @return:
        """


class PatternInterface(object):
    """编队模式"""
    def get_drift_offset(self, assignments: list) -> math2d.ndarray:
        """获得偏移量

        @return:
        """

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


class SlotAssignment(object):
    """槽位分配"""
    def __init__(self, character: CharacterInterface, slot_num: int):
        self.character = character  # 角色
        self.slot_num = slot_num  # 槽位编号


class StaticCharacter(CharacterInterface, defines.StaticObj):
    """静态角色"""
    def __init__(self):
        super(StaticCharacter, self).__init__(math2d.vector())

    def set_target(self, target: defines.Location):
        self.set_pos_vec(target.position)
        self.set_rotate(target.orientation)

    def update(self, dt):
        pass


class FormationManager(object):
    """槽位管理器"""
    def __init__(self, pattern: PatternInterface):
        self._pattern = pattern  # 编队模式
        self._slots = []  # 槽位
        self._drift_offset = defines.Location()  # 质心偏移量

    def update_slots(self, anchor_point: defines.Location):
        for slot in self._slots:
            location = self._pattern.get_slot_location(len(self._slots), slot.slot_num)
            location.position -= self._drift_offset.position
            location.orientation -= self._drift_offset.orientation
            location.position = anchor_point.position+math2d.rotate(location.position, anchor_point.orientation)
            location.orientation += anchor_point.orientation
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

    def get_drift_offset(self, assignments: list) -> defines.Location:
        position = math2d.vector()
        total_num = len(assignments)
        for assignment in assignments:
            location = self.get_slot_location(total_num, assignment.slot_num)
            position += location.position
        position = position/total_num
        return defines.Location(position)


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

    def get_drift_offset(self, assignments: list) -> defines.Location:
        position = math2d.vector()
        total_num = len(assignments)
        for assignment in assignments:
            location = self.get_slot_location(total_num, assignment.slot_num)
            position += location.position
        position = position/total_num
        return defines.Location(position)
