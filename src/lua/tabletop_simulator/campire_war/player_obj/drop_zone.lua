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
        style={}, num=0, pil_num=0,
    }
    player_passive_effect = {
        double_spell = false,
        first_invalid = false,
        play_is_life = false,
        guard = false,
        exile_lp_modify = 0,
        exile_public = 0,
        extra_fire = 0,
        extra_draw = 0,
        discount = 0,
    }
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

function hashObj(obj, ext)
    local card_info = Global.call('getCardInfo', obj)
    if not ext then ext = '' end
    return Global.call('hashConvert', card_info.name).. ext
end

function isDropOp(name)
    for _, v in ipairs({'resource', 'drop_effect', 'charge', 'loss_lp',
                        'air', 'combo', 'disadv', 'type_calc', 'style_calc',
                        'hand_calc',}) do
        if name == v then return true end
    end
    return false
end

function setPlayerEffect(params)
    for k, v in pairs(params) do
        if k ~= 'add' then
            if params.add then
                if player_passive_effect[k] == nil then
                    player_passive_effect[k] = tonumber(v)
                else
                    player_passive_effect[k] =
                        player_passive_effect[k] + tonumber(v)
                end
            else
                player_passive_effect[k] = v
            end
        end
    end
end

function getPlayerEffect()
    return player_passive_effect
end

function resetDropZoneInfo()
    played_card_info = {
        type={martial=0, spell=0, item=0, special=0},
        style={}, num=0, pil_num=0,
    }
end

function executeCardEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    -- first invalid
    if player_passive_effect.first_invalid then
        player_passive_effect.first_invalid = false
        return
    end
    calcDropZoneEffect(obj)
    calcCardEffect(obj)
    calcPlayCardInfo(obj)
end

function calcDropZoneEffect(obj)
    -- double spell
    local card_info = Global.call('getCardInfo', obj)
    if played_card_info.num == 0 and (card_info.type == 'spell' or
                                      card_info.type == 'duality') then
        if player_passive_effect.double_spell then
            calcCardEffect(obj)
            calcPlayCardInfo(obj)
        end
    end
end

function calcCardEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    local resource={}
    for k, v in pairs(card_info.resource) do
        resource[k] = v
    end
    parseCardInfo({resource=resource}, obj)
    parseCardInfo(card_info, obj)
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
        played_card_info.style[card_info.style] = 1
    else
        played_card_info.style[card_info.style] = this_style_num + 1
    end
    if player_passive_effect.play_is_life then
        local pil_num = played_card_info.pil_num + 1
        played_card_info['pil_num'] = pil_num
        if pil_num % 3 == 0 then
            local cur_lp = Global.call('getPlayerLp', self_color)
            Global.call('setPlayerLp', {color=self_color, value=cur_lp+1})
        end
    end
    played_card_info.num = played_card_info.num + 1
end

function removeItemEffect(obj)
    local card_info = Global.call('getCardInfo', obj)
    if card_info.effect.drop_effect then
        dropEffect(card_info.effect.drop_effect, true)
    end
    if card_info.keyword.drop_effect then
        dropEffect(card_info.keyword.drop_effect, true)
    end
end

function createChargeClick(charge_info, func_name)
    return function(obj, color, alt_click)
        if Global.call('isDisable') then
            return
        end
        if color ~= self_color then
            return
        end
        for fee, effect in pairs(charge_info) do
            if fee ~= 'remove' then
                local cost = tonumber(string.sub(fee, -1))
                local cur_fire = tonumber(Global.call('getPlayerFire', color))
                if cost > cur_fire then
                    break
                else
                    local new_v = cur_fire - cost
                    Global.call('setPlayerFire', {color=color, value=new_v})
                    parseCardInfo(effect, obj)
                    if charge_info.remove then
                        Global.call('removeAllUI', obj)
                    else
                        for _, btn in ipairs(obj.getButtons()) do
                            if btn.click_function == func_name then
                                obj.removeButton(btn.index)
                                break
                            end
                        end
                    end
                end
            end
        end
    end
end

function createResourceClick(resource_info)
    return function(obj, color, alt_click)
        if Global.call('isDisable') then
            return
        end
        if color ~= self_color then
            return
        end
        for k, v in pairs(resource_info) do
            local info = Global.call('getResourceInfo', k)
            local prev = tonumber(Global.call(info.getfunc, self_color))
            Global.call(info.setfunc, {color=self_color, value=prev+v})
        end
        Global.call('removeAllUI', obj)
    end
end

function createOtherClick(other_info, func_name)
    return function(obj, color, alt_click)
        if Global.call('isDisable') then
            return
        end
        if color ~= self_color then
            return
        end
        parseCardInfo(other_info, obj)
        if other_info.remove then
            Global.call('removeAllUI', obj)
        else
            for _, btn in ipairs(obj.getButtons()) do
                if btn.click_function == func_name then
                    obj.removeButton(btn.index)
                    break
                end
            end
        end
    end
end

function createBtn(params)
    params.obj.createButton({
            click_function = params.func_name,
            function_owner = self,
            position       = params.pos,
            color          = params.color,
            tooltip        = params.tooltip,
            width          = 300,
            height         = 300,
            font_size      = 250,
        })
    if not self.getVar(params.func_name) then
        if params.charge then
            local charge_info = params.charge
            self.setVar(params.func_name, createChargeClick(charge_info,
                                                            params.func_name))
        elseif params.resource then
            local resource_info = params.resource
            self.setVar(params.func_name, createResourceClick(resource_info))
        elseif params.other then
            local other_info = params.other
            self.setVar(params.func_name, createOtherClick(other_info,
                                                           params.func_name))
        end
    end
end

function charge(arg, obj)
    local card_info = Global.call('getCardInfo', obj)
    if arg.type and arg.type == 'or' then
        local index = 0
        local params = {
            color=Global.getTable('Yellow'),
            obj=obj,
        }
        for k, v in pairs(arg) do
            if k ~= 'type' then
                local cost = string.sub(k, -1)
                params['tooltip']='发动充能'.. cost
                params['pos']={index-0.5, 1, 0}
                params['func_name']=hashObj(obj, 'charge_'.. cost)
                local charge_info = {remove=true,}
                charge_info[k] = Global.call('clone', v)
                params['charge']=charge_info
                createBtn(params)
                index = index + 1
            end
        end
    else
        local params = {
            tooltip='发动充能',
            func_name=hashObj(obj, 'charge'),
            pos={0, 1, 0},
            color=Global.getTable('Yellow'),
            charge=arg,
            obj=obj,
        }
        createBtn(params)
    end
end

function air(arg, obj)
    for k, v in pairs(arg) do
        if string.find(k, 'air') then
            local fee = tonumber(string.sub(k, -1))
            local cur_fire = tonumber(Global.call(
                'getPlayerFire', self_color))
            if cur_fire >= fee then
                parseCardInfo(v, obj)
            end
        end
    end
end

function combo(arg, obj)
    local card_info = Global.call('getCardInfo', obj)
    for k, v in pairs(arg) do
        if string.find(k, 'combo') then
            local num = tonumber(string.sub(k, -1))
            local cur_num = played_card_info.type[card_info.type]
            if cur_num >= num then
                if v.type and v.type == 'or' then
                    local index = 0
                    for name, arg in pairs(v) do
                        if name ~= 'type' then
                            local tooltip = ''
                            local color = 'Green'
                            if name == 'draw' then
                                tooltip = '抽牌'
                                color = Global.getTable('Blue')
                            elseif name == 'resource' then
                                tooltip = '战力'
                                color = Global.getTable('Red')
                            end
                            local other_info = {}
                            other_info[name] = Global.call('clone', arg)
                            other_info['remove'] = true
                            local params = {
                                func_name=hashObj(obj, name),
                                tooltip=tooltip,
                                pos={index-0.5, 1, 0},
                                other=other_info,
                                color=color,
                                obj=obj,
                            }
                            createBtn(params)
                            index = index + 1
                        end
                    end
                else
                    parseCardInfo(v, obj)
                end
            end
        end
    end
end

function loss_lp(arg, obj)
    local player_colors = {}
    for _, v in ipairs(arg.object) do
        if v == 'self' then
            table.insert(player_colors, self_color)
        elseif v == 'another' then
            table.insert(player_colors, ano_color)
        end
    end
    for _, color in ipairs(player_colors) do
        local prev = tonumber(Global.call('getPlayerLp', color))
        Global.call('setPlayerLp', {color=color, value=prev-arg.value})
    end
end

function disadv(arg, obj)
    local self_lp = Global.call('getPlayerLp', self_color)
    local ano_lp = Global.call('getPlayerLp', ano_color)
    if self_lp < ano_lp then
        for k, v in pairs(arg) do
            local new_arg = {}
            new_arg[k] = Global.call('clone', v)
            parseCardInfo(new_arg, obj)
        end
    end
end

function hand_calc(arg, obj)
    local hand = getObjectFromGUID(Global.call('getColorsObjs',
                                               self_color).player_hand_zone)
    local cards = Global.call('getAboveZoneObjs', hand)
    if cards then
        for k, v in ipairs(cards) do
            parseCardInfo(arg, obj)
        end
    end
end

function type_calc(arg, obj)
    for k, v in pairs(played_card_info.type) do
        if v > 0 then
            parseCardInfo(arg, obj)
        end
    end
end

function style_calc(arg, obj)
    for k, v in pairs(played_card_info.style) do
        if v > 0 then
            parseCardInfo(arg, obj)
        end
    end
end

function resource(arg, obj)
    if arg.type then
        if arg.type == 'or' then
            local index = -1.5
            for k, v in pairs(arg) do
                if k ~= 'type' then
                    index = index + 1
                    local resource_info = {}
                    resource_info[k] = v
                    local info = Global.call('getResourceInfo', k)
                    createBtn({
                        func_name=hashObj(obj, k.. tostring(v)),
                        resource=resource_info,
                        pos={x=index, y=1, z=0},
                        tooltip=info.tooltip,
                        color=Global.getTable(info.color),
                        obj=obj,
                    })
                end
            end
            return
        end
    end
    local card_info = Global.call('getCardInfo', obj)
    for k, v in pairs(arg) do
        if k ~= 'type' then
            local info = Global.call('getResourceInfo', k)
            local prev = tonumber(Global.call(info.getfunc, self_color))
            if card_info and card_info.name == '营火' then
                v = v + player_passive_effect.extra_fire
                player_passive_effect['extra_fire'] = 0
            end
            Global.call(info.setfunc, {color=self_color, value=prev+v})
        end
    end
end

function dropEffect(arg, remove)
    local ano_drop = getObjectFromGUID(
        Global.call('getAnotherColorsObjs', self_color).player_drop_zone)
    local drop_zones = {}
    if arg.object then
        for _, v in ipairs(arg.object) do
            if v == 'self' then
                table.insert(drop_zones, self)
            elseif v == 'another' then
                table.insert(drop_zones, ano_drop)
            end
        end
    end
    for k, v in pairs(arg) do
        if k ~= 'object' then
            local params = {color=self_color}
            if string.find(k, 'extra') then params['add'] = true end
            if remove then
                params[k] = false
            else
                params[k] = v
            end
            for _, obj in ipairs(drop_zones) do
                obj.call('setPlayerEffect', params)
            end
        end
    end
end

function executeCardOp(params)
    local key = params.key
    local arg = params.arg
    local obj = params.obj
    local result = false
    if key == 'resource' then
        result = resource(arg, obj)
    elseif key == 'drop_effect' then
        result = dropEffect(arg)
    elseif key == 'charge' then
        result = charge(arg, obj)
    elseif key == 'air' then
        result = air(arg, obj)
    elseif key == 'loss_lp' then
        result = loss_lp(arg, obj)
    elseif key == 'combo' then
        result = combo(arg, obj)
    elseif key == 'disadv' then
        result = disadv(arg, obj)
    elseif key == 'type_calc' then
        result = type_calc(arg, obj)
    elseif key == 'style_calc' then
        result = style_calc(arg, obj)
    elseif key == 'hand_calc' then
        result = hand_calc(arg, obj)
    end
    return result
end

function parseCardInfo(info, obj)
    if info.keyword then
        parseCardInfo(info.effect, obj)
        parseCardInfo(info.keyword, obj)
    else
        for k, v in pairs(info) do
            if k == 'order' then
                local wait_time = 0
                for _, arg in ipairs(v) do
                    local params = {}
                    params[arg] = Global.call('clone', info[arg])
                    Wait.time(function() parseCardInfo(params, obj) end,
                              wait_time)
                    wait_time = wait_time + 1
                end
                break
            elseif isDropOp(k) then
                local arg = Global.call('clone', v)
                executeCardOp({key=k, arg=arg, obj=obj})
            elseif Global.call('isCardOp', k) then
                local arg = Global.call('clone', v)
                if k == 'draw' then
                    arg = tonumber(arg) + player_passive_effect.extra_draw
                    player_passive_effect['extra_draw'] = 0
                end
                Global.call('executeCardOp', {key=k, arg=arg, color=self_color})
            end
        end
    end
end
