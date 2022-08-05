"""路径发现算法
"""

from . import defines


def dijkstra_path_find(graph: defines.Graph, begin_node, end_node):
    """迪杰特斯拉路径发现

    @param graph:
    @param begin_node:
    @param end_node:
    @return:
    """
    def get_path(end_record: defines.NodeRecord, start):
        path = []
        while end_record.node is not start:
            begin, end = end_record.connection.from_node, end_record.connection.to_node
            path.append(defines.Connection(begin.node, end.node, end_record.connection.cost))
            end_record = end_record.connection.from_node
        return reversed(path)

    start = defines.NodeRecord(begin_node)
    open = defines.PathFindList(start)
    closed = defines.PathFindList()

    while open:
        cur = open.get()
        if cur.node is end_node:
            break
        for connection in graph.get_connections(cur.node):
            to_node = connection.to_node
            cost_so_far = cur.cost_so_far + connection.cost
            if closed.contains(to_node):
                continue
            elif open.contains(to_node):
                record = open.find(to_node)
                if record.cost_so_far <= cost_so_far:
                    continue
            else:
                record = defines.NodeRecord(to_node)
            record.cost_so_far = cost_so_far
            record.connection = defines.Connection(cur, record, connection.cost)
            not open.contains(to_node) and open.add(record)
        closed.add(cur)
        open.remove(cur)

    if cur.node is not end_node:
        return []
    else:
        return get_path(cur, begin_node)



