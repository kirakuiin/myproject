"""寻路示例
"""

import case
import math2d
import gameengine.defines as color
from gameengine import timer
from . import defines
from . import algorithm


@case.register_case(__name__)
class DijkstraPathFinding(case.Case):
    """迪杰特斯拉寻路"""
    def init_case(self):
        graph = self._generate_graph()
        self._graph_obj = defines.GraphObj(graph)
        self._graph_obj.set_pos(400, 400)
        self.add_child(self._graph_obj)
        self._timer = timer.Timer.create(self.get_shortest_path, 1, 1, graph)

    def _generate_graph(self):
        graph = defines.Graph()
        node_a = defines.NodeObj('A')
        node_b = defines.NodeObj('B')
        node_c = defines.NodeObj('C')
        node_d = defines.NodeObj('D')
        node_e = defines.NodeObj('E')
        self._begin = node_a
        self._end = node_e
        graph.add(defines.Connection(node_a, node_b, math2d.randint(1, 10)))
        graph.add(defines.Connection(node_a, node_c, math2d.randint(1, 10)))
        graph.add(defines.Connection(node_b, node_d, math2d.randint(1, 10)))
        graph.add(defines.Connection(node_c, node_d, math2d.randint(1, 10)))
        graph.add(defines.Connection(node_d, node_e, math2d.randint(1, 10)))
        graph.add(defines.Connection(node_b, node_e, math2d.randint(1, 10)))
        return graph

    def get_shortest_path(self, graph):
        path = list(algorithm.dijkstra_path_find(graph, self._begin, self._end))
        for conn in path:
            self._graph_obj.find_conn_obj(conn).set_color(*color.ORANGE)

    def update(self, dt):
        self.quit_over_time(5)