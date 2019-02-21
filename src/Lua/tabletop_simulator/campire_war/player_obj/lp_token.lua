--[[ This script is player life point script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self.createInput({
        input_function = "input_func",
        function_owner = self,
        value          = 0,
        alignment      = 1,
        position       = {x=0, y=0.1, z=0},
        width          = 800,
        height         = 800,
        font_size      = 700,
        font_color     = {r=0, g=0, b=0},
        tooltip        = '生命',
        validation     = 2,
    })
end

function input_func(obj, color, input, stillEditing)
end
