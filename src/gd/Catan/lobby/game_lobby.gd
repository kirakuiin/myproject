extends Control

# 游戏大厅

var HostItem: PackedScene = preload("res://lobby/host_item.tscn")

var _hosts: Dictionary = {}

onready var _host_group = ButtonGroup.new()
onready var _udp_server: UDPServer = UDPServer.new()
		

func _ready():
	_udp_server.listen(NetDefines.UDP_PORT)


func _on_back():
	SceneMgr.goto_scene(SceneMgr.MAIN_SCENE)


func _on_refresh_host():
	_udp_server.poll()
	if _udp_server.is_connection_available():
		var peer : PacketPeerUDP = _udp_server.take_connection()
		var name = peer.get_var()
		_add_host_item(peer.get_packet_ip(), name)
		_save_host_info(peer.get_packet_ip(), name)
		
		
func _save_host_info(ip: String, name: String):
	_hosts[ip] = name
		

func _add_host_item(ip: String, name: String):
	if ip in _hosts:
		return
	var item = HostItem.instance()
	item.set_label(name, ip)
	item.set_group(_host_group)
	$Panel/HostScroll/HostList.add_child(item)

	
func _on_refresh():
	_hosts.clear()
	for child in $Panel/HostScroll/HostList.get_children():
		$Panel/HostScroll/HostList.remove_child(child)
