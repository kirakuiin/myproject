"""寻路示例
"""

import case
import gameengine.defines as color
from gameengine import uiobject
from gameengine import timer
from . import defines
from . import algorithm


@case.register_case(__name__)
class PathFinding(case.Case):
    """寻路算法"""
    def init_case(self):
        self._grid = defines.GridGraphObj(5, 5)
        self._grid.set_pos(200, 200)
        self.add_child(self._grid)
        begin = self._grid.get_node(0, 0)
        begin.set_begin()
        end = self._grid.get_node(4, 4)
        end.set_end()
        self._timer1 = timer.Timer.create(self.get_shortest_path_dijkstra, 1, 1, begin, end)
        self._timer2 = timer.Timer.create(self.get_shortest_path_a_star, 3, 1, begin, end)

    def get_shortest_path_dijkstra(self, begin, end):
        finder = algorithm.PathFindDijkstra(self._grid.get_graph(), begin, end)
        path = finder.path_find()
        cost = 0
        for conn in path:
            conn_obj = self._grid.find_conn_obj(conn)
            conn_obj.set_color(*color.BLUE)
            cost += conn.cost
        self._text_dijk = uiobject.Text()
        self._text_dijk.set_text('dijkstra path cost: {}'.format(cost))
        self._text_dijk.set_color(*color.BLUE)
        self._text_dijk.set_pos(300, 150)
        self.add_child(self._text_dijk)
        self._show_open_close(finder.get_open_node_list(), finder.get_close_node_list())

    def _show_open_close(self, open_list, close_list):
        self._grid.reset_all()
        for node in open_list:
            node.set_open()
        for node in close_list:
            node.set_close()

    def get_shortest_path_a_star(self, begin, end):
        heuristic = defines.EuclidDistanceHeuristic(0.2)
        heuristic.set_goal(end)
        finder = algorithm.PathFindAStar(self._grid.get_graph(), begin, end, heuristic)
        path = finder.path_find()
        cost = 0
        for conn in path:
            conn_obj = self._grid.find_conn_obj(conn)
            conn_obj.set_color(*color.ORANGE)
            cost += conn.cost
        self._text_aster = uiobject.Text()
        self._text_aster.set_text('aster path cost: {}'.format(cost))
        self._text_aster.set_color(*color.ORANGE)
        self._text_aster.set_pos(300, 100)
        self.add_child(self._text_aster)
        self._show_open_close(finder.get_open_node_list(), finder.get_close_node_list())

    def update(self, dt: float):
        self.quit_over_time(5)