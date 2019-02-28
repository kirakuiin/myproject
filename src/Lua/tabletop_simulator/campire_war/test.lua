--[[ This is test script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 8, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    params = {
        click_function = "click_func",
        function_owner = self,
        label          = "test",
        width          = 600,
        height         = 600,
        font_size      = 340,
        color          = {0.5, 0.5, 0.5},
        font_color     = {1, 1, 1},
        tooltip        = "测试",
    }
    self.createButton(params)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    local drop = getObjectFromGUID(Global.call('getColorsObjs', color).player_drop_zone)
    Global.call('printTable', drop.call('getPlayerEffect'))
end
