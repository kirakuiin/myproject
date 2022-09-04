extends Control

# 玩家信息编辑

var Avatar: PackedScene = preload("res://info/player_avatar.tscn")


func _ready():
    randomize()
    _init_player_name()
    _init_avatar()
    _init_icon_grid_item()
    $Panel/VBoxContainer/IconContainer/PlayerAvatar.set_button_cb(funcref(self, "_on_change_icon"))


func _init_player_name():
    var name = PlayerConfig.get_player_name()
    $Panel/VBoxContainer/NameContainer/NameEdit.text = name


func _init_avatar():
    var icon = PlayerConfig.get_icon_path()
    $Panel/VBoxContainer/IconContainer/PlayerAvatar.set_icon(icon)


func _init_icon_grid_item():
    for path in _travesal_icon_path():
        var button = Avatar.instance()
        button.set_icon(path)
        button.set_button_cb(funcref(self, "_on_confirm_avatar"), path)
        $IconSelectScroll/IconSelectContainer.add_child(button)


func _travesal_icon_path() -> Array:
    var result = []
    var dir = Directory.new()
    if dir.open("res://assets/icons") == OK:
        dir.list_dir_begin()
        var path = dir.get_next()
        while path != "":
            if path.match("*png"):
                result.append(dir.get_current_dir()+ "/%s" % path)
            path = dir.get_next()
    return result


func _create_texture(path: String):
    var texture = ResourceLoader.load(path)
    return texture


func _on_confirm():
    SceneMgr.goto_scene(SceneMgr.MAIN_SCENE)


func _on_change_icon(path):
    $IconSelectScroll.show()


func _on_confirm_avatar(path):
    $IconSelectScroll.hide()
    $Panel/VBoxContainer/IconContainer/PlayerAvatar.set_icon(path)
    PlayerConfig.save_icon(path)


func _on_name_changed(new_text):
    PlayerConfig.save_player_name(new_text)
