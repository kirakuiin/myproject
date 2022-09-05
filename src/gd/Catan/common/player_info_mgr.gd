extends Node


signal player_added(player_info)
signal player_removed(player_info)


var _info_dict: Dictionary = {}


func add_player_info(player_info: Protocol.PlayerInfo):
	if not player_info.player_name in _info_dict:
		_info_dict[player_info.player_name] = player_info
		emit_signal("player_added", player_info)


func remove_player_by_id(id):
	for player_info in Array(_info_dict.values()):
		if player_info.peer_id == id:
			_info_dict.erase(player_info.player_name)
			emit_signal("player_removed", player_info)


func get_all_info() -> Array:
	return _info_dict.values()


func reset():
	_info_dict = {}
