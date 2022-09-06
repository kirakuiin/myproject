extends Control

# 准备大厅


const PlayerItem: PackedScene = preload("res://lobby/player_item.tscn")

var _host_info: Protocol.HostInfo


func _init():
	_host_info = Protocol.HostInfo.new(PlayerConfig.get_player_name(),
			PlayerConfig.get_icon_id(), 1, 4, Protocol.HostState.PREPARE)


func _ready():
	_init_option()
	_init_player_list()
	_init_signal()
	_init_broadcast()


func _init_option():
	for index in Data.max_player_data:
		var desc = "%s-%s人" % [Data.max_player_data[index]-1, Data.max_player_data[index]]
		$OptionContainer/PlayerNumContainer/Btn.add_item(desc, index)
	for index in Data.switch_data:
		$OptionContainer/FogContainer/Btn.add_item(Data.switch_data[index], index)
		$OptionContainer/RandResourceContainer/Btn.add_item(Data.switch_data[index], index)
		$OptionContainer/RandSeatContainer/Btn.add_item(Data.switch_data[index], index)


func _init_player_list():
	for player_info in PlayerInfoMgr.get_all_info():
		_add_player_item(player_info)


func _add_player_item(player_info: Protocol.PlayerInfo):
	var item = PlayerItem.instance()
	item.init(player_info)
	$PlayerBg/PlayerContainer.add_child(item)


func _init_signal():
	PlayerInfoMgr.connect("player_added", self, "_on_player_added")
	PlayerInfoMgr.connect("player_removed", self, "_on_player_removed")
	GameServer.connect("server_error", self, "_on_exit_prepare")


func _init_broadcast():
	if GameServer.is_server():
		$BroadcastTimer.start()


remotesync func change_max_player_num(index: int):
	$OptionContainer/PlayerNumContainer/Btn.select(index)
	_host_info.max_player_num = Data.max_player_data[index]


remotesync func change_fog_state(index: int):
	$OptionContainer/FogContainer/Btn.select(index)


func _on_broadcast():
	GameServer.broadcast(Protocol.serialize(_host_info))


func _on_exit_prepare():
	SceneMgr.goto_scene(SceneMgr.LOBBY_SCENE)
	GameServer.close_game()


func _on_change_num(index):
	rpc("change_max_player_num", index)


func _on_change_fog(index):
	rpc("change_fog_state", index)


func _on_player_added(player_info: Protocol.PlayerInfo):
	_add_player_item(player_info)
	_host_info.cur_player_num += 1


func _on_player_removed(player_info: Protocol.PlayerInfo):
	for item in $PlayerBg/PlayerContainer.get_children():
		if item.get_player_name() == player_info.player_name:
			$PlayerBg/PlayerContainer.remove_child(item)
			break
	_host_info.cur_player_num -= 1
