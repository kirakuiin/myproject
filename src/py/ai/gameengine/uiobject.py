"""可视ui对象"""

import enum
import weakref
import pygame

import math2d

from . import gameobject
from . import component
from . import camera
from . import global_vars
from . import util
from . import defines


class UIObject(gameobject.GameObject):
    """可视的ui对象"""
    def __init__(self):
        super().__init__()
        self._render_com = component.Render(self)
        self.add_component(self._render_com)

    def set_watch_num(self, num: int, is_set_child=True):
        self._render_com.set_watch_num(num, is_set_child)

    def get_watch_num(self):
        return self._render_com.get_watch_num()

    def set_visible(self, is_visible: bool):
        self._render_com.set_visible(is_visible)

    def is_visible(self) -> bool:
        return self._render_com.is_visible()

    def set_pos_vec(self, position: math2d.ndarray):
        self._set_dirty()
        super().set_pos_vec(position)

    def _set_dirty(self):
        self._render_com.set_dirty(True)

    def set_scale(self, scale: float):
        self._set_dirty()
        super().set_scale(scale)

    def set_rotate(self, rotate: float):
        self._set_dirty()
        super().set_rotate(rotate)

    def get_world_pos(self) -> math2d.ndarray:
        return self._render_com.transform.pos

    def get_world_rotate(self) -> float:
        return self._render_com.transform.rotate

    def set_color(self, r: int, g: int, b: int):
        return self._render_com.set_color(r, g, b)

    def get_color(self) -> tuple:
        return self._render_com.get_color()

    def render(self, parent_transform: math2d.Transform, dirty: bool, is_visible: bool, camera: camera.Camera):
        self._render_com.render(parent_transform, dirty, is_visible, camera)

    def draw(self, transform):
        pass

    def set_parent(self, parent):
        self._set_dirty()
        super().set_parent(parent)


class Scene(UIObject):
    """场景
    """
    def __init__(self):
        super().__init__()
        self._render_list = []  # 渲染顺序列表

    def _pre_traversal(self, node: UIObject):
        for child in node.transform.pre_children:
            self._pre_traversal(child)
        self._render_list.append(weakref.ref(node))
        for child in node.transform.post_children:
            self._pre_traversal(child)

    def get_render_order(self) -> list:
        """返回渲染顺序

        @return:
        """
        self._render_list = []
        self._pre_traversal(self)
        self._render_list = self._render_list[::-1]
        return self._render_list


class Circle(UIObject):
    """圆环
    """
    def __init__(self, radius: float, line_width=0):
        super().__init__()
        self.radius = radius  # 半径
        self.line_width = line_width  # 宽度

    def draw(self, transform):
        width = 0 if self.line_width == 0 else int(max(1, self.line_width*transform.scales[0]))
        pygame.draw.circle(global_vars.screen, self.get_color(), util.get_window_coord(transform.pos),
                           self.radius*transform.scales[0], width)


class Triangle(UIObject):
    """正三角形"""
    def __init__(self, radius):
        super().__init__()
        self.radius = radius  # 从中心到顶点的距离

    def draw(self, transform):
        pygame.draw.polygon(global_vars.screen, self.get_color(), self._calc_three_points(transform))

    def _calc_three_points(self, transform):
        top_vec = math2d.rotate(math2d.vector(0, self.radius*transform.scales[0]), transform.rotate)
        left_vec = math2d.rotate(top_vec, 120)
        right_vec = math2d.rotate(left_vec, 120)
        return [util.get_window_coord(math2d.translate(transform.pos, vec))
                for vec in [top_vec, left_vec, right_vec]]


class Lines(UIObject):
    """直线"""
    def __init__(self, second_point, *other_points, line_width=1, is_close=False):
        super().__init__()
        self.line_width = line_width
        self.points = [second_point] + list(other_points)
        self.is_close = is_close  # 是否首尾相连

    def draw(self, transform):
        pygame.draw.lines(global_vars.screen, self.get_color(), self.is_close, self._calc_points_coord(transform),
                          max(1, int(transform.scales[0]*self.line_width)))

    def _calc_points_coord(self, transform):
        result = []
        origin = self.get_pos()
        for point in [self.get_pos()]+self.points:
            vec = math2d.rotate_matrix(transform.rotate) @ math2d.scale_matrix(transform.scales) @ (point-origin)
            result.append(util.get_window_coord(transform.pos+vec))
        return result


class Text(UIObject):
    """文本"""
    def __init__(self, size=24, font_path='Arial', bold=False, italic=False):
        super().__init__()
        self._font_size = size
        self._font_path = font_path
        self._param = {'bold': bold, 'italic': italic}
        self._text = ''

    def set_text(self, text):
        """设置文本

        @param text:
        @return:
        """
        self._text = text

    def draw(self, transform):
        font_obj = self._get_font_obj(transform.scales[0])
        surface = font_obj.render(self._text, True, self.get_color())
        delta = math2d.vector(0, surface.get_height())
        global_vars.screen.blit(surface, util.get_window_coord(transform.pos+delta))

    def _get_font_obj(self, scale):
        font_size = max(1, int(self._font_size*scale))
        return pygame.font.SysFont(self._font_path, font_size, **self._param)


class FixSizeText(Text):
    """大小不随缩放改变的文本"""
    def __init__(self, size=24, font_path='Arial', bold=False, italic=False):
        super().__init__(size, font_path, bold, italic)
        self._font_obj = pygame.font.SysFont(font_path, size, bold, italic)

    def _get_font_obj(self, scale):
        return self._font_obj


class CoordLine(UIObject):
    """坐标线
    """
    MAX_VALUE = 10000000

    class LineType(enum.IntEnum):
        VERTICAL = 0
        HORIZON = 1

    @classmethod
    def create_horizon(cls, value):
        return cls(CoordLine.LineType.HORIZON, value)

    @classmethod
    def create_vertical(cls, value):
        return cls(CoordLine.LineType.VERTICAL, value)

    def __init__(self, type: LineType, value):
        super(CoordLine, self).__init__()
        self._text = FixSizeText(20)
        self._type = type
        self._init_line(value)

    def _init_line(self, value):
        self.set_value(value)
        self.adjust_value_pos(0)
        self.add_child(self._text)
        self.set_color(*defines.BLUE)
        self._text.set_color(*defines.BLUE)

    def set_value(self, value):
        """调整坐标轴的值

        @param value:
        @return:
        """
        if self._type == CoordLine.LineType.HORIZON:
            self.set_pos(-self.MAX_VALUE, value)
            self._end_point = math2d.position(self.MAX_VALUE, value)
        else:
            self.set_pos(value, -self.MAX_VALUE)
            self._end_point = math2d.position(value, self.MAX_VALUE)
        self._text.set_text(str(int(value)))

    def adjust_value_pos(self, delta):
        """调整坐标轴上坐标数值的位置

        @param delta:
        @return:
        """
        if self._type == CoordLine.LineType.HORIZON:
            self._text.set_pos(self.MAX_VALUE+delta, 0)
        else:
            self._text.set_pos(0, self.MAX_VALUE+delta)

    def draw(self, transform):
        begin, end = util.get_window_coord(transform.pos), util.get_window_coord(self._get_end_point(transform))
        pygame.draw.aaline(global_vars.screen, self.get_color(), begin, end)

    def _get_end_point(self, transform):
        vec = math2d.rotate_matrix(transform.rotate) @ math2d.scale_matrix(transform.scales) @ (self._end_point-self.get_pos())
        return transform.pos+vec


class CoordSystem(UIObject):
    """坐标系统
    """
    def __init__(self, cam: camera.Camera, space=200):
        super().__init__()
        self._cam = weakref.proxy(cam)  # 相机对象
        self._space = space  # 间隔
        self._vert_lines = {}
        self._hor_lines = {}
        self._init_camera()
        self.set_watch_num(cam.get_watch_num())
        self._update_line()

    def _init_camera(self):
        self._cam.listen_transform(self._on_camera_changed)

    def _on_camera_changed(self, transform):
        self._update_line()

    def _update_line(self):
        origin, size = self._cam.get_view_rect()
        space = self._get_space()
        vert_begin, vert_end = self._get_edge_value(origin[0]), self._get_edge_value(origin[0]+size[0])
        hor_begin, hor_end = self._get_edge_value(origin[1]), self._get_edge_value(origin[1]+size[1])
        self._fill_line(self._vert_lines, CoordLine.LineType.VERTICAL, vert_begin-space, vert_end+2*space)
        self._fill_line(self._hor_lines, CoordLine.LineType.HORIZON, hor_begin-space, hor_end+2*space)
        self._update_line_text()

    def _fill_line(self, destination, type, begin, end):
        space = self._get_space()
        range_set = set(i*space for i in range(int(begin/space), int(end/space)))
        for val in list(destination.keys()):
            range_set.remove(val) if val in range_set else destination.pop(val)
        for val in range_set:
            destination[val] = CoordLine(type, val)
            destination[val].set_watch_num(self.get_watch_num())
            self.add_child(destination[val])

    def _get_space(self):
        return self._space/self._cam.get_camera_transform().scales[0]

    def _get_edge_value(self, val):
        return val-val%self._get_space()

    def _update_line_text(self):
        origin, _ = self._cam.get_view_rect()
        for line in self._hor_lines.values():
            line.adjust_value_pos(origin[0])
        for line in self._vert_lines.values():
            line.adjust_value_pos(origin[1])
