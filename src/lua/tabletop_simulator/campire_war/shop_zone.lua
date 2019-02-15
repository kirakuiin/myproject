--[[ This script is shop slot script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ Move card from public deck to shop slot. --]]
function moveCardFromPublicDeck()
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))
    local zone_objs = public_zone.getObjects()
    if (#zone_objs > 1) then
        local deck_objs = zone_objs[2]
        if (Global.call('isDeck', deck_objs)) then
            local params = {
                position=self.getPosition(),
                top=true,
                flip=true
            }
            deck_objs.takeObject(params)
        else
            deck_objs.setPosition(self.getPosition())
            deck_objs.flip()
        end
    end
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
    if (self == zone) then
        local card_objs = self.getObjects()
        if (#card_objs == 1) then
            moveCardFromPublicDeck()
        end
    end
end
