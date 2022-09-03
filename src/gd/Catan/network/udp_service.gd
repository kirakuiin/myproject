extends Node


# 广播数据
static func broadcast(data) -> PacketPeerUDP:
	var client = PacketPeerUDP.new()
	client.set_broadcast_enabled(true)
	client.set_dest_address(NetDefines.BROADCAST_ADDR, NetDefines.UDP_PORT)
	client.put_var(data)
	return client
