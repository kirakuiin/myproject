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
                flip=true
            }
            deck_obj.takeObject(params)
        else
            deck_obj.setPosition(self.getPosition())
            deck_obj.flip()
        end
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if Global.call('isDisable') then
        return
    end
    if (self == zone) then
        local card_objs = self.getObjects()
        if (#card_objs == 1) then
            moveCardFromPublicDeck()
        end
    end
end
