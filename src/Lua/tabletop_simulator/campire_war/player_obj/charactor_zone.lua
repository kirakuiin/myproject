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

function createCardButton(obj, basic_info)
    local skill_num = basic_info.card_info.skill_num
    if skill_num > 0 then
        obj.createButton({
                click_function = "click_func",
                function_owner = self,
                position       = {x=0, y=1, z=0},
                width          = 400,
                height         = 400,
                font_size      = 300,
                alignment      = 3,
                font_color     = {1, 1, 0, 100},
                color          = {0, 0, 0, 0},
                tooltip        = "发动技能",
                label          = tostring(skill_num),
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
    end
end

function initPlayerCharacterInfo(obj)
    local lp_obj = getObjectFromGUID(lp_token)
    local basic_info = Global.call('getCardBasicInfo', obj)
    Global.call('setPlayerLp',
                {color=self_color, value=basic_info.card_info.lp})
    createCardButton(obj, basic_info)
    if Player[self_color].steam_name then
        broadcastToAll('玩家 '.. Player[self_color].steam_name.. ' 选择英雄'..
                       ' "'.. basic_info.card_info.name.. '"!',
                       Global.getTable(self_color))
    end
    initHeroSkill(obj)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    local value = tonumber(obj.getButtons()[1].label)
    if value > 0 then
        executeHeroSkill(obj)
        value = value - 1
        if Player[self_color].steam_name then
            broadcastToAll('玩家 '.. Player[self_color].steam_name..
                           ' 发动技能', Global.getTable(self_color))
        end
        if value == 0 then
            Global.call('removeAllUI', obj)
        else
            obj.editButton({index=0, label=tostring(value)})
        end
    end
end

function FiniPlayerCharacterInfo(obj)
    Global.call('setPlayerLp', {color=self_color, value=0})
    Global.call('removeAllUI', obj)
end

function initChargeButton(obj)
    local card_info = Global.call('getCardInfo', obj)
    local self_drop = getObjectFromGUID(Global.call('getColorsObjs',
        self_color).player_drop_zone)
    for k, v in pairs(card_info.skill) do
        if k == 'draw' then
            local other = {}
            other[k] = v
            local params = {
                func_name=Global.call('hashConvert', card_info.name).. k,
                other=other,
                pos={x=0, y=1, z=0},
                tooltip='ドロ一',
                color=Global.getTable('Green'),
                obj=obj,
            }
            self_drop.call('createBtn', params)
        elseif k == 'charge_3' then
            local index = 0
            for name, value in pairs(v.resource) do
                if name ~= 'type' then
                    local info = Global.call('getResourceInfo', name)
                    local resource_info = {resource={},}
                    resource_info.resource[name] = value
                    local charge_info = {}
                    charge_info[k] = resource_info
                    local params = {
                        func_name=Global.call('hashConvert', card_info.name).. name,
                        charge=charge_info,
                        pos={x=index-0.5, y=1, z=0},
                        tooltip=info.tooltip,
                        color=Global.getTable(info.color),
                        obj=obj,
                    }
                    self_drop.call('createBtn', params)
                    index = index + 1
                end
            end
        end
    end
end

function executeHeroSkill(obj)
    local card_info = Global.call('getCardInfo', obj)
    local self_drop = getObjectFromGUID(Global.call('getColorsObjs',
        self_color).player_drop_zone)
    self_drop.call('parseCardInfo', card_info.skill)
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
