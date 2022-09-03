extends Control

export(String) var broad_info = ""
var client: PacketPeerUDP = null
var _start: bool = false


func _on_click_button():
	_start = true
	print('broadcast %s' % broad_info)
	


func _on_broadcast():
	if _start:
		UdpService.broadcast(broad_info)
