--[[ This is player character script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    local objs = Global.call('getColorsObjs', self_color)
    lp_token = objs.lp_token
    on_zone_hero = {}
end

function createCardInput(obj, basic_info)
    local skill_num = basic_info.card_info.skill_num
    if skill_num > 0 then
        obj.createInput({
                input_function = "input_func",
                function_owner = self,
                value          = skill_num,
                alignment      = 3,
                position       = {x=-0.5, y=1, z=0},
                width          = 300,
                height         = 300,
                font_size      = 250,
                tooltip        = '技能次数',
                validation     = 2,
            })
    end
end

function createCardButton(obj, basic_info)
    local skill_num = basic_info.card_info.skill_num
    if skill_num > 0 then
        obj.createButton({
                click_function = "click_func",
                function_owner = self,
                position       = {x=0.5, y=1, z=0},
                width          = 300,
                height         = 300,
                font_size      = 250,
                color          = Global.getTable('Yellow'),
                tooltip        = "发动技能",
            })
    end
end

function initHeroSkill(obj)
    local hero_info = Global.call('getCardInfo', obj)
    -- passive
    if hero_info.skill_num == 0 then
        local ano_drop = getObjectFromGUID(Global.call('getAnotherColorsObjs',
                                           self_color).player_drop_zone)
        local self_drop = getObjectFromGUID(Global.call('getColorsObjs',
                                            self_color).player_drop_zone)
        if hero_info.skill.drop_effect then
            if hero_info.skill.drop_effect.exile_lp_modify then
                self_drop.call('setPlayerEffect', {exile_lp_modify=1,})
                ano_drop.call('setPlayerEffect', {exile_lp_modify=-1,})
            end
        end
    elseif hero_info.style == 'charge' then
        Global.call('removeAllUI', obj)
        initChargeButton(obj)
    else
        executeHeroSkill = function()
            if hero_info.skill.passive_effect then
            else
            end
        end
    end
end

function initPlayerCharacterInfo(obj)
    local lp_obj = getObjectFromGUID(lp_token)
    local basic_info = Global.call('getCardBasicInfo', obj)
    Global.call('setPlayerLp',
                {color=self_color, value=basic_info.card_info.lp})
    createCardInput(obj, basic_info)
    createCardButton(obj, basic_info)
    if Player[self_color].steam_name then
        broadcastToAll('玩家 '.. Player[self_color].steam_name.. ' 选择英雄'..
                       ' "'.. basic_info.card_info.name.. '"!',
                       Global.getTable(self_color))
    end
    initHeroSkill(obj)
end

function input_func(obj, color, input, stillEditing)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    local value = tonumber(obj.getInputs()[1].value)
    if value > 0 then
        executeHeroSkill()
        value = value - 1
        if Player[self_color].steam_name then
            broadcastToAll('玩家 '.. Player[self_color].steam_name..
                           ' 发动技能', Global.getTable(self_color))
        end
        if value == 0 then
            Global.call('removeAllUI', obj)
        else
            obj.editInput({index=0, value=value})
        end
    end
end

function FiniPlayerCharacterInfo(obj)
    Global.call('setPlayerLp', {color=self_color, value=0})
    Global.call('removeAllUI', obj)
end

function selectBtn1(obj, color, alt_click)
end

function selectBtn2(obj, color, alt_click)
end

function selectBtn3(obj, color, alt_click)
end

function initChargeButton(obj)
    local card_info = Global.call('getCardInfo', obj)
    for k, v in pairs(card_info.skill) do
    end
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if self == zone and Global.call('isHeroCard', obj) then
        on_zone_hero[obj.guid] = obj
    end
end

--[[ Called whenever any object is dropped by a player. --]]
function onObjectDrop(color, obj)
    if Global.call('isDisable') then
        return
    end
    if color == self_color and on_zone_hero[obj.guid] then
        initPlayerCharacterInfo(obj)
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if self == zone and Global.call('isHeroCard', obj) then
        on_zone_hero[obj.guid] = nil
        FiniPlayerCharacterInfo(obj)
    end
end
