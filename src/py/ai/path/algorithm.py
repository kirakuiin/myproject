"""路径发现算法
"""

from . import defines


class PathFindDijkstra(object):
    """迪杰特斯拉寻路算法"""
    class PathRecord(object):
        """路径记录"""
        def __init__(self, node, connection=None, cost_so_far=0):
            self.node = node
            self.connection = connection
            self.cost_so_far = cost_so_far  # 从起点到当前位置的最低代价

        def __lt__(self, other):
            return self.cost_so_far < other.cost_so_far

    def __init__(self, graph: defines.Graph, begin_node, end_node):
        self._graph = graph
        self._begin = begin_node
        self._end = end_node
        self._open_list = defines.PathFindList(self.PathRecord(begin_node))
        self._close_list = defines.PathFindList()

    def path_find(self) -> list:
        while self._open_list:
            cur_path = self._open_list.get_min()
            if cur_path.node is self._end:
                break
            self._handle_connection(cur_path)
            self._close_list.add(cur_path)
            self._open_list.remove(cur_path)
        return self._get_path(cur_path)

    def get_open_node_list(self):
        return [path.node for path in self._open_list]

    def get_close_node_list(self):
        return [path.node for path in self._close_list]

    def _handle_connection(self, cur_path):
        for connection in self._graph.get_connections(cur_path.node):
            if self._close_list.contains(connection.to_node):
                self._in_close_op(cur_path, connection)
            elif self._open_list.contains(connection.to_node):
                self._in_open_op(cur_path, connection)
            else:
                self._not_found_op(cur_path, connection)

    def _in_close_op(self, cur_path, connection):
        pass

    def _in_open_op(self, cur_path, connection):
        path = self._open_list.find(connection.to_node)
        cost_so_far = cur_path.cost_so_far + connection.cost
        if path.cost_so_far > cost_so_far:
            path.cost_so_far = cost_so_far
            path.connection = defines.Connection(cur_path, path, connection.cost)

    def _not_found_op(self, cur_path, connection):
        new_path = self.PathRecord(connection.to_node)
        new_path.cost_so_far = cur_path.cost_so_far + connection.cost
        new_path.connection = defines.Connection(cur_path, new_path, connection.cost)
        self._open_list.add(new_path)

    def _get_path(self, path) -> list:
        path_list = []
        if path.node is self._end:
            while path.node is not self._begin:
                begin, end = path.connection.from_node, path.connection.to_node
                path_list.append(defines.Connection(begin.node, end.node, path.connection.cost))
                path = path.connection.from_node
        return list(reversed(path_list))


def path_find_dijkstra(graph: defines.Graph, begin_node, end_node) -> list:
    """迪杰特斯拉路径发现

    @param graph: 图
    @param begin_node: 起点
    @param end_node: 终点
    @return:
    """
    finder = PathFindDijkstra(graph, begin_node, end_node)
    return finder.path_find()


class PathFindAStar(PathFindDijkstra):
    """A*路径发现算法
    """
    class PathRecord(object):
        """路径记录"""
        def __init__(self, node, connection=None, cost_so_far=0, estimate=0):
            self.node = node
            self.connection = connection
            self.cost_so_far = cost_so_far  # 从起点到当前位置的最低代价
            self.estimate_total_cost = estimate  # 估算总代价

        def __lt__(self, other):
            return self.estimate_total_cost < other.estimate_total_cost

    def __init__(self, graph, begin_node, end_node, heuristic: defines.HeuristicInterface):
        super().__init__(graph, begin_node, end_node)
        self._heuristic = heuristic

    def _in_close_op(self, cur_path, connection):
        path = self._close_list.find(connection.to_node)
        cost_so_far = cur_path.cost_so_far + connection.cost
        if path.cost_so_far > cost_so_far:
            self._close_list.remove(path)
            path.estimate_total_cost = (path.estimate_total_cost-path.cost_so_far)+cost_so_far
            path.cost_so_far = cost_so_far
            path.connection = defines.Connection(cur_path, path, connection.cost)
            self._open_list.add(path)

    def _in_open_op(self, cur_path, connection):
        path = self._open_list.find(connection.to_node)
        cost_so_far = cur_path.cost_so_far + connection.cost
        if path.cost_so_far > cost_so_far:
            path.estimate_total_cost = (path.estimate_total_cost-path.cost_so_far)+cost_so_far
            path.cost_so_far = cost_so_far
            path.connection = defines.Connection(cur_path, path, connection.cost)

    def _not_found_op(self, cur_path, connection):
        new_path = self.PathRecord(connection.to_node)
        cost_so_far = cur_path.cost_so_far + connection.cost
        new_path.cost_so_far = cost_so_far
        new_path.connection = defines.Connection(cur_path, new_path, connection.cost)
        new_path.estimate_total_cost = cost_so_far + self._heuristic.estimate(new_path.node)
        self._open_list.add(new_path)


def path_find_a_star(graph: defines.Graph, begin_node, end_node, heuristic: defines.HeuristicInterface) -> list:
    """a*路径发现

    @param graph:
    @param begin_node:
    @param end_node:
    @param heuristic: 启发式算法类
    @return:
    """
    finder = PathFindAStar(graph, begin_node, end_node, heuristic)
    return finder.path_find()


def smooth_path(input_path: list, ray_cast) -> list:
    """路径平滑算法

    @param input_path:
    @param ray_cast: 射线检测函数
    @return: 平滑后的路径
    """
    output_path = [input_path.pop(0)]
    for idx, path in enumerate(input_path[1:]):
        if not ray_cast(output_path[-1], path):
            output_path.append(input_path[idx])
    output_path.append(input_path[-1])
    return output_path