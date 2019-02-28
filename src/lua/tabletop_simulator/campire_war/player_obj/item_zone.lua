--[[ This is player item zone script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    self_drop_guid = Global.call('getColorsObjs', self_color).player_drop_zone
    on_zone_item = {}
    is_equiping = false
end

function createCardInput(obj, card_info)
    local defense = card_info.defense
    if not obj.getInputs() then
        obj.createInput({
                input_function = "input_func",
                function_owner = self,
                value          = defense,
                alignment      = 3,
                position       = {x=-0.5, y=1, z=0},
                width          = 400,
                height         = 400,
                font_size      = 300,
                font_color     = {1, 1, 0, 100},
                color          = {0, 0, 0, 0},
                tooltip        = '装备耐久',
                validation     = 2,
            })
    end
end

function createCardButton(obj, card_info)
    if not obj.getButtons() then
        obj.createButton({
                click_function = "click_func",
                function_owner = self,
                position       = {x=0.5, y=1, z=0},
                width          = 300,
                height         = 300,
                font_size      = 250,
                color          = Global.getTable('Red'),
                tooltip        = "攻击装备[对方点击]",
            })
    end
end

function createPeresvet(obj, card_info)
    if not obj.getButtons() then
        obj.createButton({
                click_function = "add_minus",
                function_owner = self,
                position       = {x=0, y=1, z=0},
                width          = 800,
                height         = 800,
                font_size      = 600,
                alignment      = 3,
                font_color     = {1, 1, 0, 100},
                color          = {0, 0, 0, 0},
                tooltip        = "充能点数",
                label          = "0",
            })
    end
end

function initPlayerItemInfo(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.name == '佩里夫特' then
        createPeresvet(obj, card_info)
    else
        createCardInput(obj, card_info)
        createCardButton(obj, card_info)
    end
    if Player[self_color].steam_name then
        broadcastToAll('玩家 '.. Player[self_color].steam_name.. ' 装备了'..
                       ' "'.. card_info.name.. '"!',
                       Global.getTable(self_color))
    end
end

function input_func(obj, color, input, stillEditing)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    local ano_color = Global.call('getAnotherColor', self_color)
    if color ~= ano_color then
        return
    end
    local power = tonumber(Global.call('getPlayerPower', ano_color))
    local defense = tonumber(obj.getInputs()[1].value)
    local diff = power - defense
    if diff < 0 then
        Global.call('setPlayerPower', {color=ano_color, value=0})
        obj.editInput({index=0, value=defense-power})
    else
        Global.call('setPlayerPower', {color=ano_color, value=diff})
        Global.call('moveItemToDiscard', {color=self_color, guid=self.guid})
    end
end

function add_minus(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    local val = alt_click and -1 or 1
    local cur_val = tonumber(obj.getButtons()[1].label)
    local new_val = cur_val + val
    if new_val < 0 then new_val = 0 end
    obj.editButton({index=0, label=tostring(new_val)})
end

function finiPlayerItemInfo(obj)
    Global.call('removeAllUI', obj)
    local card_info = Global.call('getCardInfo', obj)
    if Player[self_color].steam_name then
        broadcastToAll('玩家 '.. Player[self_color].steam_name..
                       ' 移除了装备 "'.. card_info.name.. '"!',
                       Global.getTable(self_color))
    end
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if (self == zone) and self_color == turn_color then
        if Global.call('isItemCard', obj) or
            Global.call('getCardInfo', obj).name == '佩里夫特' then
            on_zone_item[obj.guid] = obj
        end
    end
end

--[[ Called whenever any object is dropped by a player. --]]
function onObjectDrop(color, obj)
    local turn_color = Global.call('getGameInfo').turn_color
    if Global.call('isDisable') then
        return
    end
    if color == self_color and color == turn_color and
        on_zone_item[obj.guid] then
        initPlayerItemInfo(obj)
        drop_zone = getObjectFromGUID(self_drop_guid)
        drop_zone.call('executeCardEffect', obj)
        is_equiping = true
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if self == zone then
        if Global.call('isItemCard', obj) or
            Global.call('getCardInfo', obj).name == '佩里夫特' then
            on_zone_item[obj.guid] = nil
            if is_equiping then
                finiPlayerItemInfo(obj)
                is_equiping = false
                drop_zone = getObjectFromGUID(self_drop_guid)
                drop_zone.call('removeItemEffect', obj)
            end
        end
    end
end
