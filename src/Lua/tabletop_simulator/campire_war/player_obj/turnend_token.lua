--[[ This script is turn end button script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    ano_color = Global.call('getAnotherColor', self_color)
    local objs = Global.call('getAnotherColorsObjs', self_color)
    other_btn_guid = objs.turn_end_token
    other_drop_guid = objs.player_drop_zone
end

function getPlayerAllItem(color)
    local objs = Global.call('getColorsObjs', color)
    local item_zone_1 = getObjectFromGUID(objs.player_item_zone_one)
    local item_zone_2 = getObjectFromGUID(objs.player_item_zone_two)
    local items = {}
    items[objs.player_item_zone_one]=Global.call('getAboveZoneObj' ,item_zone_1)
    items[objs.player_item_zone_two]=Global.call('getAboveZoneObj' ,item_zone_2)
    for k,item in pairs(items) do
        if not Global.call('isItemCard', item) then
            items[k] = nil
        end
    end
    return items
end

function exchangeBtn()
    Global.call('removeButton', self)
    other_btn = getObjectFromGUID(other_btn_guid)
    params = {
        click_function = "click_func",
        function_owner = other_btn,
        width          = 800,
        height         = 800,
        font_size      = 400,
        font_color     = Global.getTable(self_color),
        tooltip        = "回合结束",
    }
    other_btn.createButton(params)
end

function setGlobalInfo()
    Global.call('setGameInfo', {turn_color=ano_color})
end

function checkGuard()
    local ano_drop = getObjectFromGUID(other_drop_guid)
    local effect = ano_drop.call('getPlayerEffect')
    if effect.guard then
        local power = tonumber(Global.call('getPlayerPower',
            self_color))
        local defense = 3
        local guid = ''
        for k, v in pairs(getPlayerAllItem(ano_color)) do
            local card_info = Global.call('getCardInfo', v)
            if card_info.name == '心源道服' then
                defense = v.getInputs()[1].value
                guid = k
                break
            end
        end
        local diff = power - defense
        if diff < 0 then
            Global.call('setPlayerPower', {color=self_color, value=0})
        else
            Global.call('setPlayerPower',
                {color=self_color, value=diff})
            Global.call('moveItemToDiscard',
                {color=ano_color, guid=guid, ready=false})
        end
    end
end

function resetPlayerResource()
    -- reset player fire
    Global.call('setPlayerFire', {color=self_color, value=0})
    -- reset item life
    for _, item in pairs(getPlayerAllItem(self_color)) do
        if item then
            local defense = Global.call('getCardInfo', item).defense
            item.editInput({index=0, value=defense})
            -- remove skill button
            local self_drop_guid = Global.call('getColorsObjs',
                self_color).player_drop_zone
            local self_drop = getObjectFromGUID(self_drop_guid)
            for _, btn in ipairs(item.getButtons()) do
                if btn.function_owner == self_drop then
                    item.removeButton(btn.index)
                end
            end
        end
    end
    for _, item in pairs(getPlayerAllItem(ano_color)) do
        if item then
            local defense = Global.call('getCardInfo', item).defense
            item.editInput({index=0, value=defense})
        end
    end
    -- attack
    local power = tonumber(Global.call('getPlayerPower', self_color))
    Global.call('setPlayerPower', {color=self_color, value=0})
    local another_lp = tonumber(Global.call('getPlayerLp', ano_color))
    local diff = another_lp - power
    if diff > 0 then
        Global.call('setPlayerLp', {color=ano_color, value=diff})
    else
        Global.call('setPlayerLp', {color=ano_color, value=0})
        if Player[ano_color].steam_name then
            broadcastToAll('玩家'.. Player[ano_color].steam_name.. ' 卒!',
                Global.getTable(ano_color))
        end
    end
end

function setAnoPlayerInitResource()
    for k, item in pairs(getPlayerAllItem(ano_color)) do
        if item then
            local ano_drop = getObjectFromGUID(other_drop_guid)
            ano_drop.call('calcCardEffect', item)
        end
    end
end

function resetDropZoneInfo()
    local self_drop_guid = Global.call('getColorsObjs',
                                       self_color).player_drop_zone
    local self_drop = getObjectFromGUID(self_drop_guid)
    self_drop.call('resetDropZoneInfo')
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    -- if another player has heart clothes
    checkGuard()
    -- reset player all resource
    resetPlayerResource()
    resetDropZoneInfo()
    Global.call('turnEndDropAndDraw', self_color)
    exchangeBtn()
    setGlobalInfo()
    if Player[self_color].steam_name then
        broadcastToAll("玩家 "..Player[self_color].steam_name.. " 回合结束!",
            Global.getTable(self_color))
    end
    setAnoPlayerInitResource()
end
