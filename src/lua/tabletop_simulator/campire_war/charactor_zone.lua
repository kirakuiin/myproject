--[[ This script is player character script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

function initPlayerCharacterInfo(basic_info)
    print(basic_info.card_info.name)
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    if self == zone and Global.call('isCard', obj) then
        local basic_info = Global.call('getCardBasicInfo', obj)
        if Global.call('isHero', basic_info) then
            initPlayerCharacterInfo(basic_info)
        end
    end
end
