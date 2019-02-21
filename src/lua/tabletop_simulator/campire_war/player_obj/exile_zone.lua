--[[ This script is exile zone script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    ano_color = Global.call('getAnotherColor', self_color)
    on_zone_cards = {}
    exile_cards_info = {brick=0}
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if (self == zone) and self_color == turn_color then
        if Global.call('isCard', obj) then
            on_zone_cards[obj.guid] = obj
        end
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if (self == zone) and self_color == turn_color then
        if Global.call('isCard', obj) then
            on_zone_cards[obj.guid] = nil
        end
    end
end

--[[ Called whenever any object is dropped by a player. --]]
function onObjectDrop(color, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if color == self_color and self_color == turn_color and
        on_zone_cards[obj.guid] then
        executeExileEffect(obj)
    end
end

function executeExileEffect(obj)
    calcDropZoneEffect(obj)
    calcExileEffect(obj)
    calcPlayCardInfo(obj)
end

function calcDropZoneEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    drop = getObjectFromGUID(Global.call('getColorsObjs',
                             self_color).player_drop_zone)
    local effect = drop.call('getPlayerEffect')
    local modify = effect.exile_lp_modify
    if card_info.style == 'basic' and modify ~= 0 then
        local lp = tonumber(Global.call('getPlayerLp', self_color))
        Global.call('setPlayerLp', {color=self_color, value=lp + modify})
    end
end

function calcExileEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.keyword.exile then
        local exile = card_info.keyword.exile
        for k, v in pairs(exile) do
            if k == 'resource' then setExileResource(v) end
            if k == 'brick' then setBrickDamage() end
        end
    end
end

function calcPlayCardInfo(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.name == '板砖' then
        exile_cards_info['brick'] = exile_cards_info['brick'] + 1
    end
end

function setExileResource(resource)
    for k, v in pairs(resource) do
        local info = Global.call('getResourceInfo', k)
        local prev = tonumber(Global.call(info.getfunc, self_color))
        Global.call(info.setfunc, {color=self_color, value=prev+v})
    end
end

function setBrickDamage()
    local damage = exile_cards_info.brick + 1
    local lp = Global.call('getPlayerLp', ano_color)
    Global.call('setPlayerLp', {color=ano_color, value=lp-damage})
end
