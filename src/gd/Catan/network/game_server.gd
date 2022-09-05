extends Node


signal connection_failed()
signal connection_succeeded()
signal server_error()
	

var peer = null


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
	PlayerInfoMgr.add_player_info(Protocol.create_player_info_by_id(get_peer_id()))


# 加入主机
func join_game(ip: String):
	peer = NetworkedMultiplayerENet.new()
	peer.create_client(ip, NetDefines.GAME_PORT)
	get_tree().set_network_peer(peer)
	PlayerInfoMgr.add_player_info(Protocol.create_player_info_by_id(get_peer_id()))
	
	
# 关闭连接
func close_game():
	if peer:
		peer.close_connection()
		PlayerInfoMgr.reset()
		
	
# 发出玩家相关信号
func emit_player(signal_name: String, player_info: Protocol.PlayerInfo):
	emit_signal(signal_name, player_info)


# 判断是否为服务器
func is_server() -> bool:
	return get_tree().is_network_server()


# 获得自身peer id
func get_peer_id() -> int:
	return get_tree().get_network_unique_id()


# 同步玩家信息
remote func sync_player_info():
	var info = Protocol.PlayerInfo.new(PlayerConfig.get_player_name(),
			get_tree().get_network_unique_id(), PlayerConfig.get_icon_id())
	rpc("register_player_info", Protocol.serialize(info))


remote func register_player_info(net_data):
	PlayerInfoMgr.add_player_info(Protocol.deserialize(net_data))
	
	
remotesync func remove_player_info(player_id):
	PlayerInfoMgr.remove_player_by_id(player_id)


# 请求其他玩家信息
master func request_all_player_info(client_id):
	var data = Protocol.serialize(PlayerInfoMgr.get_all_info())
	rpc_id(client_id, "recv_all_player_info", data)	


remote func recv_all_player_info(net_datas: Array):
	var info_list = Protocol.deserialize(net_datas)
	for player_info in info_list:
		PlayerInfoMgr.add_player_info(player_info)

# all
func _on_player_connected(id):
	if is_server():
		rpc_id(id, "sync_player_info")
	elif id == NetDefines.SERVER_ID:
		rpc_id(id, "request_all_player_info", get_tree().get_network_unique_id())


func _on_player_disconnected(id):
	rpc("remove_player_info", id)


# clients only
func _on_connected_ok():
	emit_signal("connection_succeeded")


func _on_server_disconnected():
	PlayerInfoMgr.reset()
	emit_signal("server_error")


func _on_connected_fail():
	get_tree().set_network_peer(null)
	emit_signal("connection_failed")


