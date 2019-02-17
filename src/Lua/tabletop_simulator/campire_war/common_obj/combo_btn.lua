--[[ This is style select script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    public_pile_guid = Global.getVar('public_card_pile')
    combo_bag = Global.getVar('combo_bag')
    params = {
        click_function = "click_func",
        function_owner = self,
        label          = "combo",
        width          = 1200,
        height         = 1200,
        font_size      = 340,
        color          = {0.5, 0.5, 0.5},
        font_color     = {1, 1, 1},
        tooltip        = "选择连击风格",
    }
    self.createButton(params)
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    local bag = getObjectFromGUID(combo_bag)
    local public_zone = getObjectFromGUID(public_pile_guid)
    if bag.getQuantity() == 1 then
        Global.call('addNewStyle', combo_bag)
        broadcastToAll('Select combo style!', {r=0, g=1, b=0})
        local params = {
            position=public_zone.getPosition(),
            rotation={0, 0, 0},
            top=true,
            smooth=false,
        }
        bag.takeObject(params)

        if #Global.call('getStyle') == 3 then
            local watchFunc = function()
                local above_obj = Global.call('getAboveZoneObj', public_zone)
                if (above_obj) then
                    -- make sure two object coincide
                    if Global.call('isCloseEnough', {obj1=public_zone,
                            obj2=above_obj}) then
                        return true
                    end
                end
                return false
            end

            local toRunFunc = function()
                local deck = Global.call('getAboveZoneObj', public_zone)
                if #Global.call('getStyle') == 3 then
                    deck.flip()
                end
            end

            Wait.condition(toRunFunc, watchFunc, Global.getVar('timeout'))
        end

    end
end
