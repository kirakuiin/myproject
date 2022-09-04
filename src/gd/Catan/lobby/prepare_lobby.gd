extends Control

# 准备大厅

const max_num_map = {
    0: 4,
    1: 6,
   }

var _host_info: Protocol.HostInfo


func _init():
    _host_info = Protocol.HostInfo.new(PlayerConfig.get_player_name(), PlayerConfig.get_icon_path(), 1, 4, Protocol.HostState.PREPARE)


func _ready():
    _init_option()
    _init_server()


func _init_server():
    GameServer.host_game()


func _init_option():
    for index in max_num_map:
        var desc = "%s-%s人" % [index-1, index]
        $OptionContainer/PlayerNumContainer/PlayerNumBtn.add_item(desc, index)


func _on_broadcast():
    GameServer.broadcast(_host_info.to_var())


func _on_exit_prepare():
    SceneMgr.goto_scene(SceneMgr.LOBBY_SCENE)


func _on_change_num(index):
    _host_info.max_player_num = max_num_map[index]
