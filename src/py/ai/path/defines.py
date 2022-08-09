"""路径基础对象定义
"""

import bisect
import heapq
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


class HeuristicInterface(object):
    """启发式算法"""
    def set_goal(self, to_node):
        """设置启发算法的终点

        @param to_node:
        @return:
        """

    def estimate(self, from_node) -> float:
        """返回由起点到终点的估计成本

        @param from_node:
        @return:
        """


class EuclidDistanceHeuristic(HeuristicInterface):
    """欧几里得距离

    估算值为两者坐标之间的距离
    """
    def __init__(self, scale=1.0):
        self._goal = None
        self._scale = scale

    def set_goal(self, to_node):
        self._goal = to_node

    def estimate(self, from_node) -> float:
        return math2d.distance(from_node.get_pos(), self._goal.get_pos())*self._scale


class PathFindList(object):
    """路径发现列表

    由最小堆实现
    """
    def __init__(self, first=None):
        self._path_list = []
        first and self.add(first)

    def __iter__(self):
        return iter(self._path_list)

    def __len__(self):
        return len(self._path_list)

    def __bool__(self):
        return bool(len(self))

    def add(self, path):
        heapq.heappush(self._path_list, path)

    def remove(self, path):
        self._path_list.remove(path)
        heapq.heapify(self._path_list)

    def get_min(self):
        return self and self._path_list[0]

    def contains(self, path) -> bool:
        return bool(self.find(path))

    def find(self, path):
        for record in self._path_list:
            if record.node is path:
                return record
        else:
            return None


class NodeObj(uiobject.Circle):
    """表示节点的图形显示"""
    ORIGIN_COLOR = color.WHITE
    def __init__(self):
        super().__init__(10)
        self._flag_obj = None
        self._origin_color = self.ORIGIN_COLOR

    def set_begin(self):
        self._origin_color = color.RED
        self.set_color(*self._origin_color)

    def set_end(self):
        self._origin_color = color.GREEN
        self.set_color(*self._origin_color)

    def reset(self):
        self.set_color(*self._origin_color)

    def set_open(self):
        if self._origin_color == self.ORIGIN_COLOR:
            self.set_color(*color.YELLOW)

    def set_close(self):
        if self._origin_color == self.ORIGIN_COLOR:
            self.set_color(*color.PURPLE)



class ConnectionObj(uiobject.UIObject):
    """连接对象"""
    def __init__(self, from_node, to_node, cost):
        super().__init__()
        begin, end = from_node.get_pos(), to_node.get_pos()
        self._line = uiobject.Lines.create_line(begin, end)
        self._text = uiobject.Text()
        self.add_child(self._line)
        self.add_child(self._text)
        self._text.set_text(str(cost))
        self._text.set_color(*color.BLACK)
        self._text.set_pos_vec((begin+end)/2-math2d.vector(10, 10))

    def __repr__(self):
        return '{}({})'.format(self.__class__.__name__, self._text.get_text())

    def set_color(self, r: int, g: int, b: int):
        self._line.set_color(r, g, b)


class GridGraphObj(uiobject.UIObject):
    """网格图对象"""
    SPACE = 100
    def __init__(self, w, h):
        super().__init__()
        self._nodes = [[NodeObj() for i in range(h)] for j in range(w)]
        self._connections = {}
        self._graph = Graph()
        self._init_node_pos(w, h)
        self._fill_graph(w, h)

    def _init_node_pos(self, w, h):
        for i in range(w):
            for j in range(h):
                self._nodes[i][j].set_pos(i*self.SPACE, j*self.SPACE)
                self.add_child(self._nodes[i][j])

    def _fill_graph(self, w, h):
        for i in range(w):
            for j in range(h):
                node = self._nodes[i][j]
                if i < w-1:
                    self._add_connection(node, self._nodes[i+1][j])
                if j < h-1:
                    self._add_connection(node, self._nodes[i][j+1])

    def _add_connection(self, from_node, to_node):
        cost = math2d.randint(1, 50)
        conn = Connection(from_node, to_node, cost)
        connObj = ConnectionObj(from_node, to_node, cost)
        self._graph.add(conn)
        self._connections[conn] = connObj
        self.add_child(connObj)

    def get_graph(self):
        return self._graph

    def find_conn_obj(self, connection):
        for conn in self._connections:
            if conn == connection:
                return self._connections[conn]

    def get_node(self, i, j):
        return self._nodes[i][j]

    def reset_all(self):
        for nodes in self._nodes:
            for node in nodes:
                node.reset()