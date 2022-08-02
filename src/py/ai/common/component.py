"""组件基类"""

import weakref


class Component(object):
    """组件"""
    def __init__(self, parent=None):
        self._comp_parent = parent and weakref.ref(parent) or None
        self._comp_children = []  # 组件列表

    def __str__(self):
        return '\n'.join(self._display(0))

    def _display(self, level, indent=2):
        info = ['{}{}'.format('-'*level*indent, self.__class__.__name__)]
        for comp in self._comp_children:
            info.extend(comp._display(level+1, indent))
        return info

    @property
    def root(self):
        """返回根组件

        @return:
        """
        root_comp = self
        while root_comp.parent:
            root_comp = root_comp.parent
        return root_comp

    @property
    def parent(self):
        """返回父组件

        @return:
        """
        return self._comp_parent and self._comp_parent()

    def get_component(self, comp_cls):
        """获得子组件

        @param comp_cls:
        @return:
        """
        for comp in self._comp_children:
            if isinstance(comp, comp_cls):
                return comp

    def get_components(self, comp_cls) -> list:
        """获得组件列表

        @param comp_cls:
        @return:
        """
        return [comp for comp in self._comp_children if isinstance(comp, comp_cls)]

    def add_component(self, comp):
        """添加子组件

        @param comp:
        """
        assert isinstance(comp, Component), "追加对象必须为{}的子类".format(Component.__name__)
        self._comp_children.append(comp)

    def remove_component(self, comp_cls):
        """移除组件

        @param comp_cls:
        @return:
        """
        for comp in self._comp_children[::]:
            if isinstance(comp, comp_cls):
                comp.release()
                self._comp_children.remove(comp)

    def get_component_in_parent(self, comp_cls):
        """从父组件中获得组件

        @param comp_cls:
        @return:
        """
        if self.parent:
            return self.parent.get_component(comp_cls) or self.parent.get_component_in_parent(comp_cls)

    def get_components_in_parent(self, comp_cls) -> list:
        """从父组件中获得组件列表

        @param comp_cls:
        @return:
        """
        result = []
        if self.parent:
            result.extend(self.parent.get_components(comp_cls))
            result.extend(self.parent.get_components_in_parent(comp_cls))
        return result

    def get_component_in_children(self, comp_cls):
        """从子组件中获得指定组件

        @param comp_cls:
        @return:
        """
        for comp in self._comp_children:
            result = comp.get_component(comp_cls)
            if not result:
                result = comp.get_component_in_children(comp_cls)
            if result:
                return result

    def get_components_in_children(self, comp_cls) -> list:
        """从子组件中获得组件列表

        @param comp_cls:
        @return:
        """
        result = []
        for comp in self._comp_children:
            result.extend(comp.get_components(comp_cls))
            result.extend(comp.get_components_in_children(comp_cls))
        return result

    def release(self):
        """释放组件

        @return:
        """
        for comp in self._comp_children:
            comp.release()
        self._comp_children = []