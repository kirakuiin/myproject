"""路径基础对象定义
"""

import bisect
import collections

import math2d

from gameengine import uiobject
from gameengine import defines as color


class Connection(object):
    """连接"""
    def __init__(self, from_node, to_node, cost):
        self.from_node = from_node
        self.to_node = to_node
        self.cost = cost

    def __repr__(self):
        return '{}({}--{}-->{})'.format(self.__class__.__name__, self.from_node, self.cost, self.to_node)

    def __eq__(self, other):
        return self.from_node is other.from_node and self.to_node is other.to_node and self.cost == other.cost

    def __hash__(self):
        return id(self)


class Graph(object):
    """图"""
    def __init__(self):
        self._connection_map = collections.defaultdict(list)

    def add(self, connection: Connection):
        self._connection_map[connection.from_node].append(connection)

    def get_all_node(self) -> list:
        result = set()
        for node in self._connection_map:
            for connect in self.get_connections(node):
                result.add(connect.from_node)
                result.add(connect.to_node)
        return list(result)

    def get_connections(self, from_node) -> list:
        """返回从指定节点为起点的所有连接

        @param from_node:
        @return:
        """
        return self._connection_map[from_node]


class NodeRecord(object):
    """节点记录"""
    def __init__(self, node, connection=None, cost_so_far=0):
        self.node = node
        self.connection = connection
        self.cost_so_far = cost_so_far  # 从起点到当前位置的最低代价

    def __repr__(self):
        return '{}({}, cost={})'.format(self.__class__.__name__, self.node, self.cost_so_far)


class PathFindList(object):
    """路径发现列表"""
    def __init__(self, first=None):
        self._path_list = []
        first and self.add(first)

    def __len__(self):
        return len(self._path_list)

    def __bool__(self):
        return bool(len(self))

    def add(self, record: NodeRecord):
        bisect.insort(self._path_list, record, key=lambda node: node.cost_so_far)

    def remove(self, record: NodeRecord):
        self._path_list.remove(record)

    def get(self):
        """返回最小成本记录
        @rtype: NodeRecord
        """
        return self and self._path_list[0]

    def contains(self, node) -> bool:
        return bool(self.find(node))

    def find(self, node):
        """根据节点返回记录

        @rtype: NodeRecord
        @param node:
        """
        for record in self._path_list:
            if record.node is node:
                return record
        else:
            return None


class NodeObj(uiobject.UIObject):
    """表示节点的图形显示"""
    def __init__(self, name):
        super().__init__()
        self._circle = uiobject.Circle(20)
        self._circle.set_color(*color.RED)
        self._text = uiobject.Text()
        self._text.set_text(name)
        self._text.set_color(*color.BLACK)
        self._text.set_pos(-8, -12)
        self.add_child(self._circle)
        self.add_child(self._text)

    def __repr__(self):
        return '{}({})'.format(self.__class__.__name__, self._text.get_text())


class ConnectionObj(uiobject.UIObject):
    """连接对象"""
    def __init__(self, from_node, to_node, cost):
        super().__init__()
        begin, end = from_node.get_pos(), to_node.get_pos()
        self._line = uiobject.Lines.create_line(begin, end)
        self.add_child(self._line)
        self._text = uiobject.Text()
        self._text.set_text(str(cost))
        self._text.set_color(*color.BLACK)
        self._text.set_pos_vec((begin+end)/2-math2d.vector(10, 10))
        self.add_child(self._text)

    def __repr__(self):
        return '{}({})'.format(self.__class__.__name__, self._text.get_text())

    def set_color(self, r: int, g: int, b: int):
        self._line.set_color(r, g, b)


class GraphObj(uiobject.UIObject):
    """图显示对象"""
    def __init__(self, graph: Graph):
        super().__init__()
        self._node_list = []
        self._conn_dict = {}
        self._init_graph(graph)

    def _init_graph(self, graph: Graph):
        self._node_list = graph.get_all_node()
        for node in self._node_list:
            self.add_child(node)
        self._init_node_pos()
        self._init_connection(graph)

    def _init_node_pos(self):
        for idx, node_obj in enumerate(self._node_list):
            base_node = self._node_list[max(0, idx-1)]
            pos = base_node.get_pos()
            rand_vec = math2d.normalize(math2d.vector(math2d.rand_bio(), math2d.rand_bio()))*200
            node_obj.set_pos_vec(pos+rand_vec)

    def _init_connection(self, graph):
        for node in graph.get_all_node():
            for conn in graph.get_connections(node):
                conn_obj = ConnectionObj(conn.from_node, conn.to_node, conn.cost)
                self._conn_dict[conn] = conn_obj
                self.add_child(conn_obj, -1)

    def find_conn_obj(self, connection):
        for conn in self._conn_dict:
            if conn == connection:
                return self._conn_dict[conn]