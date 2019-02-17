--[[ This is first hand script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print("Player first hand button onLoad!")
    local other_objs = Global.call('getAnotherColorsObjs',
                             Global.call('getObjsColor', self))
    local self_objs = Global.call('getColorsObjs',
                             Global.call('getObjsColor', self))

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
            if not zone.call('isHeroCard', card) then
                return false
            end
        else
            return false
        end
    end
    return true
end

function addTwoLife()
    local lp_obj = getObjectFromGUID(ano_lp_zone)
    Global.call('modifyInputValue', {obj=lp_obj, offset=2})
end

function removeFirstHandBtn()
    another = getObjectFromGUID(another_btn)
    another.removeButton(0)
    another.setInvisibleTo(getSeatedPlayers())
    self.removeButton(0)
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

function createEndBtn(color)
    end_btn = getObjectFromGUID(end_btn_guid)
    params = {
        click_function = "click_func",
        function_owner = end_btn,
        width          = 800,
        height         = 800,
        font_size      = 400,
        font_color     = Global.getTable(color),
        tooltip        = "回合结束",
    }
    end_btn.createButton(params)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if color ~= Global.call('getObjsColor', self) then
        return
    end
    if (not Global.call('isValidColor', color)) then
        broadcastToAll('Please select a player color.',
                        Global.getTable('Orange'))
        return
    end
    if not isPlayerAllReady() then
        broadcastToAll('Some player not place hero card.',
                        Global.getTable('Orange'))
        return
    end
    addTwoLife()
    removeFirstHandBtn()
    shuffleAndDraw()
    createEndBtn(color)
    broadcastToAll("Player " .. Player[color].steam_name.. " get first hand",
                   Global.getTable(color))
end
