--[[ This script is shop slot script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ Move card from public deck to shop slot. --]]
function moveCardFromPublicDeck()
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))
    deck_obj = Global.call('getAboveZoneObj', public_zone)
    if deck_obj then
        if (Global.call('isDeck', deck_obj)) then
            local params = {
                position=self.getPosition(),
                top=true,
                flip=true,
                smooth=false,
            }
            deck_obj.takeObject(params)
        else
            deck_obj.setPosition(self.getPosition())
            deck_obj.flip()
        end
    end
end

function sellCard(picking_info)
    if not Global.call('isValidColor', picking_info.color) then
        return
    end
    local objs = Global.call('getColorsObjs', picking_info.color)
    local fire = tonumber(Global.call('getPlayerFire', picking_info.color))
    local drop = getObjectFromGUID(objs.player_drop_zone)
    local effect = drop.call('getPlayerEffect')
    local card_value = Global.call('getCardInfo', picking_info.obj).value
    fire = fire - card_value
    if effect.discount then
        fire = fire + effect.discount
        drop.call('setPlayerEffect', {discount=nil})
    end
    Global.call('setPlayerFire', {color=picking_info.color, value=fire})
end

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if (self == zone) then
        if not Global.call('getAboveZoneObj', self) then
            if Global.call('isCard', obj) then
                save_card = obj
            end
        end
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if (self == zone) then
        local picking_info = Global.call('getPickingInfo')
        if picking_info.obj == obj and obj == save_card then
            sellCard(picking_info)
        end
        if (not Global.call('getAboveZoneObj', self)) then
            save_card = nil
            moveCardFromPublicDeck()
        end
    end
end

function onLoad()
    save_card = nil
end
