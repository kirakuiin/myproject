--[[ This is test script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 8, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    params = {
        click_function = "click_func",
        function_owner = self,
        label          = "disable",
        width          = 600,
        height         = 600,
        font_size      = 340,
        color          = {0.5, 0.5, 0.5},
        font_color     = {1, 1, 1},
        tooltip        = "关闭脚本",
    }
    self.createButton(params)
end

function click_func(obj, color, alt_click)
    local disable = Global.call('isDisable')
    if disable then
        Global.call('enableAllScript')
        self.editButton({index=0, tooltip='关闭脚本'})
        broadcastToAll('关闭所有脚本.', {r=0, g=0, b=1})
    else
        Global.call('disableAllScript')
        self.editButton({index=0, tooltip='打开脚本'})
        broadcastToAll('启动所有脚本.', {r=1, g=0, b=0})
    end
end
