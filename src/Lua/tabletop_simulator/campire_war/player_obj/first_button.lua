--[[ This is first hand script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    self_color = Global.call('getObjsColor', self)
    local other_objs = Global.call('getAnotherColorsObjs', self_color)
    local self_objs = Global.call('getColorsObjs', self_color)

    ano_lp_zone = other_objs.lp_token
    another_btn = other_objs.first_hand_token
    end_btn_guid = self_objs.turn_end_token
    player_zone = Global.getTable('player_zone')

    params = {
        click_function = "click_func",
        function_owner = self,
        width          = 800,
        height         = 800,
        font_size      = 340,
        font_color     = Global.getTable('Black'),
        tooltip        = "抢先手",
    }
    self.createButton(params)
end

function isPlayerAllReady()
    for _, player_guid in ipairs(player_zone) do
        local zone = getObjectFromGUID(player_guid)
        local card = Global.call('getAboveZoneObj', zone)
        if card then
            if not Global.call('isHeroCard', card) then
                return false
            end
        else
            return false
        end
    end
    return true
end

function addTwoLife()
    local ano_color = Global.call('getAnotherColor', self_color)
    local current_life = Global.call('getPlayerLp', ano_color)
    Global.call('setPlayerLp', {color=ano_color, value=current_life+2})
end

function removeFirstHandBtn()
    another = getObjectFromGUID(another_btn)
    Global.call('removeButton', another)
    another.setInvisibleTo(getSeatedPlayers())
    Global.call('removeButton', self)
    self.setInvisibleTo(getSeatedPlayers())
end

function shuffleAndDraw()
    for _, color in ipairs(getSeatedPlayers()) do
        local card_guid = Global.call('getColorsObjs', color).player_card_pile
        local card_zone = getObjectFromGUID(card_guid)
        local color = Global.call('getObjsColor', card_zone)
        local deck = Global.call('getAboveZoneObj', card_zone)
        if deck then
            deck.shuffle()
            Global.call('drawCard', {color=color, num=5})
        end
    end
end

function createEndBtn()
    end_btn = getObjectFromGUID(end_btn_guid)
    params = {
        click_function = "click_func",
        function_owner = end_btn,
        width          = 800,
        height         = 800,
        font_size      = 400,
        font_color     = Global.getTable(self_color),
        tooltip        = "回合结束",
    }
    end_btn.createButton(params)
end

function setGlobalInfo()
    Global.call('setGameInfo', {turn_color=self_color})
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= self_color then
        return
    end
    if (not Global.call('isValidColor', self_color)) then
        broadcastToAll('请选择一个玩家颜色[红/蓝]!',
                        Global.getTable('Orange'))
        return
    end
    if not isPlayerAllReady() then
        broadcastToAll('有玩家没有选择英雄卡!',
                        Global.getTable('Orange'))
        return
    end
    addTwoLife()
    removeFirstHandBtn()
    shuffleAndDraw()
    createEndBtn()
    setGlobalInfo()
    broadcastToAll("玩家 " .. Player[self_color].steam_name.. " 获得先手!",
                   Global.getTable(self_color))
end
