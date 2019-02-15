--[[ This is first hand script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

lp_zone = ''
another_btn = ''

player_zones = {'7cc7c6', 'b6dbb6'}

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print("Player's first hand button onLoad!")
    params = {
        click_function = "click_func",
        function_owner = self,
        width          = 800,
        height         = 800,
        font_size      = 340,
        color          = {0.5, 0.5, 0.5},
        font_color     = {1, 1, 1},
        tooltip        = "抢先手",
    }
    self.createButton(params)
end

function isPlayerAllReady()
    for _, player_guid in ipairs(player_zones) do
        local zone = getObjectFromGUID(player_guid)
        local objs = zone.getObjects()
        if #objs > 1 then
            if not zone.call('isHeroCard', objs[2]) then
                return false
            end
        else
            return false
        end
    end
    return true
end

function addTwoLife()
    local lp_obj = getObjectFromGUID(lp_zone)
    local new_lp = lp_obj.getInputs()[1].value + 2
    lp_obj.editInput({index=0, value=new_lp})
end

function removeFirstHandBtn()
    another = getObjectFromGUID(another_btn)
    another.removeButton(0)
    another.setInvisibleTo(getSeatedPlayers())
    self.removeButton(0)
    self.setInvisibleTo(getSeatedPlayers())
end

function enableTurn()
    Turns.enable = true
end

function click_func(obj, color, alt_click)
    if (color == 'White' or color == 'Grey') then
        broadcastToAll('Please select a player color.',
                        {r=1, g=0, b=0})
        return
    end
    if not isPlayerAllReady() then
        broadcastToAll('Some player not place hero card.',
                        {r=1, g=0, b=0})
        return
    end
    addTwoLife()
    removeFirstHandBtn()
    enableTurn()
end
