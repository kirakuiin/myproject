extends Control

# 主机列表项

export(Color) var highlight_color
export(Color) var origin_color


func set_label(name: String, ip: String) -> void:
	no_highlight()
	$HostNameLabel.text = name
	$IPLabel.text = ip
	
	
func set_group(group: ButtonGroup):
	$SelectBtn.group = group


func get_ip():
	return $IPLabel.text
	
	
func highlight():
	$HostNameLabel.add_color_override("font_color", highlight_color)
	$IPLabel.add_color_override("font_color", highlight_color)


func no_highlight():
	$HostNameLabel.add_color_override("font_color", origin_color)
	$IPLabel.add_color_override("font_color", origin_color)


func _on_select(button_pressed: bool):
	if button_pressed:
		highlight()
	else:
		no_highlight()
		
