"""游戏对象基类
"""

import math2d

from . import component


class GameObject(component.EngineComponent):
    """游戏对象类
    """
    def __init__(self):
        super().__init__()
        self._transform_com = component.Transform(self)
        self.add_component(self._transform_com)

    def __repr__(self):
        return '{}(pos={})'.format(self.__class__.__name__, self.get_pos())

    @property
    def transform(self):
        return self._transform_com

    def get_order(self) -> int:
        return self._transform_com.get_order()

    def set_pos(self, x: float, y: float):
        self.set_pos_vec(math2d.position(x, y))

    def set_pos_vec(self, position: math2d.ndarray):
        self._transform_com.transform.pos = math2d.position(position[0], position[1])

    def get_pos(self) -> math2d.ndarray:
        return self._transform_com.transform.pos

    def set_scale(self, scale: float):
        self._transform_com.transform.scales = math2d.array((scale, scale))

    def get_scale(self) -> math2d.ndarray:
        return self._transform_com.transform.scales

    def set_rotate(self, rotate: float):
        self._transform_com.transform.rotate = rotate

    def get_rotate(self) -> float:
        return self._transform_com.transform.rotate

    def update(self, dt: float):
        pass

    def add_child(self, child, order=0):
        self._transform_com.add_child(child, order)

    def remove_child(self, child):
        self._transform_com.remove_child(child)

    def get_parent(self):
        return self._transform_com.get_parent()

    def set_parent(self, parent):
        self._transform_com.set_parent(parent)

    def get_hierarchy(self):
        return '\n'.join(self._hierarchy(0))

    def _hierarchy(self, level, indent=2):
        info = ['{}{}'.format('-'*level*indent, self.__class__.__name__)]
        for child in self._transform_com.children:
            info.extend(child._hierarchy(level+1, indent))
        return info
