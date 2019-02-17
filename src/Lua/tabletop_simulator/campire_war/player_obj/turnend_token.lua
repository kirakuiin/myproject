--[[ This script is turn end button script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    local objs = Global.call('getAnotherColorsObjs',
                             Global.call('getObjsColor', self))
    other_btn_guid = objs.turn_end_token
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    local self_color = Global.call('getObjsColor', self)
    if color ~= self_color then
        return
    end
    Global.call('turnEndOp', color)
    broadcastToAll("Player "..Player[color].steam_name.. "'s turn end",
                   Global.getTable(color))

    self.removeButton(0)
    other_btn = getObjectFromGUID(other_btn_guid)
    params = {
        click_function = "click_func",
        function_owner = other_btn,
        width          = 800,
        height         = 800,
        font_size      = 400,
        font_color     = Global.getTable(color),
        tooltip        = "回合结束",
    }
    other_btn.createButton(params)
end
