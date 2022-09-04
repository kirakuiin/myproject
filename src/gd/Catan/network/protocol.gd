extends Node

# 网络协议


class ProtocolData:
    extends Reference

    var cls_name: String = "ProtocolData"

    func _to_string() -> String:
        var string = "%s{" % cls_name
        for property in get_property_list():
            if is_export(property.name):
                string += "%s=%s, " % [property.name, get(property.name)]
        string += "}"
        return string

    # 转为网络协议需要的数据
    func to_var() -> Dictionary:
        var result = {}
        for property in get_property_list():
            if is_export(property.name):
                result[property.name] = get(property.name)
        return result

    # 加载数据到类中
    func load_var(data: Dictionary):
        for name in data:
            if is_export(name):
                set(name, data[name])

    func is_export(name: String):
        return false


enum HostState {PREPARE, PLAYING}


# 主机信息
class HostInfo:
    extends ProtocolData

    var host_name: String
    var cur_player_num: int
    var max_player_num: int
    var icon_path: String
    var host_state: int

    func _init(name, icon, cur_num, max_num, state):
        host_name = name
        cur_player_num = cur_num
        max_player_num = max_num
        host_state = state
        icon_path = icon
        cls_name = "HostInfo"

    func is_export(name: String):
        return name in ["host_name", "cur_player_num", "max_player_num", "host_state", "icon_path"]


# 创建主机信息
static func CreateHostInfo(data) -> HostInfo:
    var info = HostInfo.new("", "", 0, 0, HostState.PREPARE)
    info.load_var(data)
    return info


# 玩家信息
class PlayerInfo:
    extends ProtocolData

    var player_name: String
    var peer_id: int
    var icon_path: String

    func _init(name, id, path):
        player_name = name
        peer_id = id
        icon_path = path
        cls_name = "PlayerInfo"

    func is_export(name: String):
        return name in ["player_name", "peer_id", "icon_path"]
