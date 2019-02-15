--[[ This script is global script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 8, 2019
-- license: GPL.v3

--[[ Global scriptzone guid --]]

heros_zone = 'e948da'

public_card_pile = 'f212eb'
campire_card_pile = 'f0982c'
shop_slot_1 = '3c0ad6'
shop_slot_2 = '22f8e4'
shop_slot_3 = '5dd3ea'
shop_slot_4 = '379a88'
shop_slot_5 = '883dae'
public_exile = 'a07c4b'

player_card_pile_1 = '02f99b'
player_drop_zone_1 = '9b7b5b'
player_discard_pile_1 = '69b312'
player_exile_pile_1 = 'a268e1'
player_character_zone_1 = '7cc7c6'
player_item_zone_one_1 = 'd0013e'
player_item_zone_two_1 = '99e36b'
player_hand_zone_1 = 'df1711'

player_card_pile_2 = 'b24d55'
player_drop_zone_2 = '998d3b'
player_discard_pile_2 = '064c48'
player_exile_pile_2 = 'e033fb'
player_character_zone_2 = 'b6dbb6'
player_item_zone_one_2 = '8e051a'
player_item_zone_two_2 = 'b8fd56'
player_hand_zone_2 = '495d24'

shop_zone = {
    shop_slot_1, shop_slot_2, shop_slot_3, shop_slot_4, shop_slot_5
}

drop_zone = {
    player_drop_zone_1, player_drop_zone_2
}

hand_zone = {
    player_hand_zone_1, player_hand_zone_2
}

--[[ Hero card guid --]]

ben = 'f24598'
bingmei = 'dc6553'
aguan = 'db57bb'
docter = 'dd2fb2'
frog = '77090a'
luna = '31405f'
roam = 'cfcadf'

hero_table = {
    ben, bingmei, aguan, docter, frog, luna, roam
}

--[[ Utility code --]]

function isCard(obj)
    return obj.tag == 'Card'
end

function isHero(basic_info)
    return basic_info.type == 'hero'
end

function isDeck(obj)
    return obj.tag == 'Deck'
end

function printTable(tab_obj)
    for k, v in ipairs(tab_obj) do
        print('key='.. k.. ' v='.. v.tag)
    end
end

function getCardBasicInfo(obj)
    if obj then
        return obj.getTable('basic_info')
    end
    return nil
end

--[[ Game build-in event --]]

--[[ Called when any object enters any scripting zone. --]]
function onObjectEnterScriptingZone(zone, obj)
end

--[[ Called when any object leaves any scripting zone. --]]
function onObjectLeaveScriptingZone(zone, obj)
end

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print('Global onLoad!')
    for k, v in ipairs({player_character_zone_1, player_character_zone_2}) do
        local obj = getObjectFromGUID(v)
        if (obj.script_code == '') then
            obj.script_code = '--[[]]'
        end
    end
end

--[[ The onUpdate event is called once per frame. --]]
function onUpdate()
    --[[ print('onUpdate loop!') --]]
end
