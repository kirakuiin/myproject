--[[ This is player character script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

lp_token = ''
power_token = ''
fire_token = ''

function isHeroCard(obj)
    if Global.call('isCard', obj) then
        local basic_info = Global.call('getCardBasicInfo', obj)
        if basic_info.type == 'hero' then
            return true
        end
    end
    return false
end

function initPlayerCharacterInfo(obj)
    local lp_obj = getObjectFromGUID(lp_token)
    local basic_info = Global.call('getCardBasicInfo', obj)
    lp_obj.editInput({index=0, value=basic_info.card_info.lp})
    obj.createInput({
        input_function = "input_func",
        function_owner = lp_obj,
        label          = basic_info.card_info.skill_num,
        alignment      = 3,
        position       = {x=0, y=1, z=0},
        width          = 300,
        height         = 300,
        font_size      = 250,
        tooltip        = '技能次数',
        validation     = 2,
    })
end

function FiniPlayerCharacterInfo(obj)
    local lp_obj = getObjectFromGUID(lp_token)
    lp_obj.editInput({index=0, value=0})
    obj.removeInput(0)
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    if self == zone and isHeroCard(obj) then
        initPlayerCharacterInfo(obj)
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if self == zone and isHeroCard(obj) then
        FiniPlayerCharacterInfo(obj)
    end
end
