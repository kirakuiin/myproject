--[[ This script is player fire point script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    self.createButton({
        click_function = 'click_func',
        function_owner = self,
        position       = {0, 0.1, 0},
        height         = 800,
        width          = 800,
        tooltip        = '营火',
        alignment      = 3,
        scale          = {x=1.5, y=1.5, z=1.5},
        font_size      = 600,
        font_color     = {0.95, 0.39, 0.11, 100},
        color          = {0, 0, 0, 0},
        label          = '0'
    })
end

function click_func(obj, color, alt_click)
    local val = alt_click and -1 or 1
    local cur_val = Global.call('getPlayerFire', self_color)
    local new_val = cur_val + val
    if new_val < 0 then new_val = 0 end
    Global.call('setPlayerFire', {color=self_color, value=new_val})
end
