extends Node

# 游戏状态管理

var game_state: InnerStateInterface = PrepareState.new()  # 游戏状态


func _ready():
    GameServer.connect("connection_failed", self, "_on_connection_failed")
    GameServer.connect("server_refused", self, "_on_server_refused")
    GameServer.connect("client_disconnected", self, "_on_client_disconnected")
    GameServer.connect("client_connected", self, "_on_client_connected")


# 切换至准备状态
func to_prepare():
    game_state = PrepareState.new()


# 切换至游戏状态
func to_playing(player_names: Array):
    game_state = PlayingState.new(player_names)


# 是否接受连接
func is_accept_connection(player_info: Protocol.PlayerInfo) -> bool:
    return game_state.is_accept_connection(player_info)


func set_max_conn(num: int):
    game_state.set_max_conn(num)


func set_cur_conn(num: int):
    game_state.set_cur_conn(num)


# 内部状态
class InnerStateInterface:
    extends Reference

    func is_accept_connection(player_info: Protocol.PlayerInfo) -> bool:
        return true

    func get_state() -> int:
        return 0

    func set_max_conn(num: int):
        pass

    func set_cur_conn(num: int):
        pass

# 准备状态
class PrepareState:
    extends InnerStateInterface

    var _max_conn: int = 3
    var _cur_conn: int = 0

    func is_accept_connection(player_info: Protocol.PlayerInfo) -> bool:
        return _cur_conn < _max_conn

    func get_state() -> int:
        return Protocol.HostState.PREPARE

    func set_max_conn(num: int):
        _max_conn = num

    func set_cur_conn(num: int):
        _cur_conn = num


# 游玩状态
class PlayingState:
    extends InnerStateInterface

    var _player_names: Array

    func _init(player_names: Array):
        _player_names = player_names

    func is_accept_connection(player_info: Protocol.PlayerInfo) -> bool:
        return player_info.player_name in _player_names

    func get_state() -> int:
        return Protocol.HostState.PLAYING


func _on_server_refused():
    to_prepare()


func _on_client_disconnected(peer_id: int):
    pass


func _on_client_connected(player_info: Protocol.PlayerInfo):
    pass


func _on_connection_failed():
    to_prepare()