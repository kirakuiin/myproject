--[[ This script is drop zone script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    ano_color = Global.call('getAnotherColor', self_color)
    on_zone_cards = {}
    played_card_info = {
        type={martial=0, spell=0, item=0, special=0},
        style={}, num=0
    }
    player_passive_effect = {
        double_spell = false,
        first_invalid = false,
        exile_lp_modify = 0,
        guard = false,
        exile_public = false,
        discount = false,
    }
    prev_fire = 0
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if (self == zone) and self_color == turn_color then
        if Global.call('isNonItemCard', obj) then
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
        if Global.call('isNonItemCard', obj) then
            on_zone_cards[obj.guid] = nil
            Global.call('removeAllUI', obj)
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
        executeCardEffect(obj)
    end
end

function setPlayerEffect(params)
    for k, v in pairs(params) do
        player_passive_effect[k] = v
    end
end

function getPlayerEffect()
    return player_passive_effect
end

function resetDropZoneInfo()
    played_card_info = {
        type={martial=0, spell=0, item=0, special=0},
        style={}, num=0
    }
    prev_fire = 0
end

function executeCardEffect(obj)
    calcDropZoneEffect(obj)
    calcCardResource(obj)
    calcCardEffect(obj)
    calcCardKeyword(obj)
    calcPlayCardInfo(obj)
end

function calcDropZoneEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    -- first invalid
    if player_passive_effect.first_invalid then
        player_passive_effect.first_invalid = false
        return
    end
    -- double spell
    if played_card_info.num == 0 and card_info.type == 'spell' then
        if player_passive_effect.double_spell then
            executeCardEffect(obj)
        end
    end
end

function selectBtn1(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    for _, btn in pairs(obj.getButtons()) do
        if btn.click_function == 'selectBtn1' then
            local v = tonumber(btn.label)
            local info = Global.call('getResourceInfo', btn.tooltip)
            local prev = tonumber(Global.call(info.getfunc, self_color))
            Global.call(info.setfunc, {color=self_color, value=prev+v})
            break
        end
    end
    Global.call('removeButton', obj)
end

function selectBtn2(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    for _, btn in pairs(obj.getButtons()) do
        if btn.click_function == 'selectBtn2' then
            local v = tonumber(btn.label)
            local info = Global.call('getResourceInfo', btn.tooltip)
            local prev = tonumber(Global.call(info.getfunc, self_color))
            Global.call(info.setfunc, {color=self_color, value=prev+v})
            break
        end
    end
    Global.call('removeButton', obj)
end

function createSelectBtn(obj, resource)
    if not obj.getButtons() then
        local x_pos = -1.5
        local index = 0
        for k, v in pairs(resource) do
            index = index + 1
            x_pos = x_pos + 1
            local info = Global.call('getResourceInfo', k)
            local color = info.color
            local tooltip = info.tooltip
            obj.createButton({
                    click_function = "selectBtn".. index,
                    function_owner = self,
                    position       = {x=x_pos, y=1, z=0},
                    width          = 300,
                    height         = 300,
                    font_size      = 250,
                    color          = Global.getTable(color),
                    tooltip        = tooltip,
                    label          = tostring(v),
                })
        end
    end
end

function calcCardResource(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.resource.type then
        local tp = card_info.resource.type
        card_info.resource['type'] = nil
        if tp == 'or' then
            createSelectBtn(obj, card_info.resource)
            return
        end
    end
    for k, v in pairs(card_info.resource) do
        local info = Global.call('getResourceInfo', k)
        local prev = tonumber(Global.call(info.getfunc, self_color))
        Global.call(info.setfunc, {color=self_color, value=prev+v})
        if k == 'fire' then prev_fire = prev end
    end
end

function calcCardEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
end

function calcCardKeyword(obj)
    local card_info = Global.call('getCardInfo', obj)
end

function calcPlayCardInfo(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.type ~= nil then
        if card_info.type == 'duality' then
            played_card_info.type.spell = played_card_info.type.spell + 1
            played_card_info.type.martial = played_card_info.type.martial + 1
        else
            local this_type_num = played_card_info.type[card_info.type]
            played_card_info.type[card_info.type] = this_type_num + 1
        end
    end
    local this_style_num = played_card_info.style[card_info.style]
    if not this_style_num then
        played_card_info.style[card_info.style] = 0
    else
        played_card_info.style[card_info.style] = this_style_num + 1
    end
    played_card_info.num = played_card_info.num + 1
end

function removeItemEffect(obj)
end
