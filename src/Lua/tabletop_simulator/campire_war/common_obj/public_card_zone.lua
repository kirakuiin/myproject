--[[ This is public card zone script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 8, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print('Public card zone onLoad!')
    start_btn_guid = Global.getVar('quick_start_token')
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    if self == zone then
        if Global.call('isCard', obj) or Global.call('isDeck', obj) then
            local start_btn = getObjectFromGUID(start_btn_guid)
            start_btn.editButton({index=0, tooltip='商店分卡&分配英雄.'})
        end
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if self == zone then
        if #self.getObjects() == 1 then
            local start_btn = getObjectFromGUID(start_btn_guid)
            start_btn.editButton({index=0, tooltip='随机选择三种风格快速游戏.'})
        end
    end
end
