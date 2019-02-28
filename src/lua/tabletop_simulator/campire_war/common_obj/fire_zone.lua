--[[ This is fire fire zone script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if (self == zone) then
        if Global.call('isCard', obj) then
            local card_info = Global.call('getCardInfo', obj)
            if card_info.name == '营火火' then
                local color = Global.call('getPickingInfo').color
                local cur_val = Global.call('getPlayerFire', color)
                Global.call('setPlayerFire', {color=color,
                                              value=cur_val-card_info.value})
            end
        end
    end
end
