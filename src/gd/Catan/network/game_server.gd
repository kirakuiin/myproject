extends Node


signal player_added(player_info)
signal player_removed(player_info)
signal connection_failed()
signal connection_succeeded()
signal server_error(what)


# 玩家信息管理
class PlayerInfoMgr:
    extends Reference

    var _info_dict: Dictionary = {}

    func add_player_info(player_info: Protocol.PlayerInfo):
        _info_dict[player_info.player_name] = player_info

    func remove_player_by_id(id):
        for player_info in Array(_info_dict.values()):
            if player_info.peer_id == id:
                _info_dict.erase(player_info.name)

    func reset():
        _info_dict = {}


var peer = null
onready var player_mgr: PlayerInfoMgr = PlayerInfoMgr.new()


func _ready():
    get_tree().connect("network_peer_connected", self, "_on_player_connected")
    get_tree().connect("network_peer_disconnected", self,"_on_player_disconnected")
    get_tree().connect("connected_to_server", self, "_on_connected_ok")
    get_tree().connect("connection_failed", self, "_on_connected_fail")
    get_tree().connect("server_disconnected", self, "_on_server_disconnected")


# 广播数据
static func broadcast(data) -> PacketPeerUDP:
    var client = PacketPeerUDP.new()
    client.set_broadcast_enabled(true)
    client.set_dest_address(NetDefines.BROADCAST_ADDR, NetDefines.BROAD_PORT)
    client.put_var(data)
    return client


# 创建主机
func host_game():
    peer = NetworkedMultiplayerENet.new()
    peer.create_server(NetDefines.GAME_PORT, NetDefines.MAX_PEER)
    get_tree().set_network_peer(peer)


# 加入主机
func join_game(ip: String):
    peer = NetworkedMultiplayerENet.new()
    peer.create_client(ip, NetDefines.GAME_PORT)
    get_tree().set_network_peer(peer)


# 同步玩家信息
remote func sync_player_info():
    rpc("register_player_info")


remotesync func register_player_info():
    var info = Protocol.PlayerInfo.new(PlayerConfig.get_player_name(),
            get_tree().get_network_unique_id(), PlayerConfig.get_icon_path())
    player_mgr.add_player_info(info)
    emit_signal("player_changed")


# all
func _on_player_connected(id):
    if get_tree().is_network_server():
        rpc_id(id, "sync_player_info")


func _on_player_disconnected(id):
    pass


# clients only
func _on_connected_ok():
    emit_signal("connection_succeeded")


func _on_server_disconnected():
    player_mgr.reset()
    emit_signal("game_error", "Server disconnected")


func _on_connected_fail():
    player_mgr.reset()
    get_tree().set_network_peer(null)
    emit_signal("connection_failed")


