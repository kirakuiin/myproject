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
		return name.match("?*_?*")
		
		
# 类名:类映射
static func get_cls(cls_name) -> ProtocolData:
	var map = {
		"HostInfo": HostInfo,
		"PlayerInfo": PlayerInfo,
	}
	return map[cls_name]
		

# 序列化数据
static func serialize(datas):
	var result = datas
	if datas is ProtocolData:
		result = datas.to_var()
	elif datas is Array:
		result = []
		for data in datas:
			result.append(serialize(data))
	elif datas is Dictionary:
		result = {}
		for key in datas:
			result[key] = serialize(datas[key])
	return result
		
		
# 反序列化数据
static func deserialize(net_data):
	var result = net_data
	if net_data is Dictionary:
		if "cls_name" in net_data:
			result = get_cls(net_data["cls_name"]).new()
			result.load_var(net_data)
		else:
			result = {}
			for key in net_data:
				result[key] = deserialize(net_data[key])
	elif net_data is Array:
		result = []
		for data in net_data:
			result.append(deserialize(data))
	return result
		

enum HostState {PREPARE, PLAYING}


# 主机信息
class HostInfo:
	extends ProtocolData

	var host_name: String
	var cur_player_num: int
	var max_player_num: int
	var icon_id: int
	var host_state: int

	func _init(name="", icon=1, cur_num=0, max_num=0, state=HostState.PREPARE):
		cls_name = "HostInfo"
		host_name = name
		cur_player_num = cur_num
		max_player_num = max_num
		host_state = state
		icon_id = icon


# 玩家信息
class PlayerInfo:
	extends ProtocolData

	var player_name: String
	var peer_id: int
	var icon_id: int

	func _init(name="", id=0, icon=0):
		cls_name = "PlayerInfo"
		player_name = name
		peer_id = id
		icon_id = icon
		

# 创建玩家数据
static func create_player_info_by_id(peer_id: int) -> PlayerInfo:
	return PlayerInfo.new(PlayerConfig.get_player_name(), peer_id, PlayerConfig.get_icon_id())