extends Control

# 游戏大厅

signal joined_game()

var HostItem: PackedScene = preload("res://lobby/host_item.tscn")

var _hosts: Dictionary = {}

onready var _host_group = ButtonGroup.new()
onready var _udp_server: UDPServer = UDPServer.new()


func _ready():
	_udp_server.listen(NetDefines.BROAD_PORT)
	GameServer.connect("connection_succeeded", self, "_on_enter_prepare")


func _on_back():
	SceneMgr.goto_scene(SceneMgr.MAIN_SCENE)


func _on_refresh_host():
	print('oooopes')
	_udp_server.poll()
	if _udp_server.is_connection_available():
		var peer : PacketPeerUDP = _udp_server.take_connection()
		var data = peer.get_var()
		if data:
			var host_info = Protocol.deserialize(data) as Protocol.HostInfo
			_add_host_item(peer.get_packet_ip(), host_info)
			_save_host_info(peer.get_packet_ip(), host_info)


func _save_host_info(ip: String, info: Protocol.HostInfo):
	_hosts[ip] = info


func _add_host_item(ip: String, info: Protocol.HostInfo):
	if ip in _hosts:
		return
	$Panel/HostScroll/HostList.add_child(_create_item(ip, info))


func _create_item(ip: String, info: Protocol.HostInfo):
	var item = HostItem.instance()
	item.set_host_name(info.host_name)
	item.set_num(info.cur_player_num, info.max_player_num)
	item.set_ip(ip)
	item.set_state(info.host_state)
	item.set_group(_host_group)
	item.set_join_signal(self)
	return item


func _on_refresh():
	_hosts.clear()
	for child in $Panel/HostScroll/HostList.get_children():
		$Panel/HostScroll/HostList.remove_child(child)


func _on_create_lobby():
	GameServer.host_game()
	SceneMgr.goto_scene(SceneMgr.PREPARE_SCENE)


func _on_join_game():
	emit_signal("joined_game")


func _on_enter_prepare():
	SceneMgr.goto_scene(SceneMgr.PREPARE_SCENE)