--[[ This script quick start script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

qigong_bag = '1d2a03'
combo_bag = 'fcde40'
mix_bag = 'a26060'
spirit_bag = '293ca0'
fight_bag = '363384'
charge_bag = '12b623'

common_decks = {
    qigong_bag, combo_bag, mix_bag, spirit_bag, fight_bag, charge_bag,
}

timeout = 5

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print('Start button onLoad!')
    params = {
        click_function = "click_func",
        function_owner = self,
        label          = "start game",
        width          = 1200,
        height         = 1200,
        font_size      = 340,
        color          = {0.5, 0.5, 0.5},
        font_color     = {1, 1, 1},
        tooltip        = "随机选择三种风格快速游戏",
    }
    self.createButton(params)
end

function move_card_pile_to_public(public_zone)
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))
    local selected_pile = 0
    local selected_style = {}

    while selected_pile ~= 3 do
        local num = math.random(1,6)
        local pile = getObjectFromGUID(common_decks[num])
        if (pile.getQuantity() == 1) then
            local params = {
                position=public_zone.getPosition(),
                rotation={0, 0, 0},
                top=true,
            }
            pile.takeObject(params)
            selected_pile = selected_pile + 1
            table.insert(selected_style, common_decks[num])
        end
    end

    local watchFunc = function()
        local objs = public_zone.getObjects()
        if (#objs > 1) then
            local deck = public_zone.getObjects()[2]
            if deck.getQuantity() == 60 then
                return true
            end
        end
        return false
    end
    local toRunFunc = function()
        local deck = public_zone.getObjects()[2]
        deck.flip()
        deck.shuffle()
    end
    Wait.condition(toRunFunc, watchFunc, timeout)

    return selected_style
end

function put_card_to_shop()
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))

    local watchFunc = function()
        local objs = public_zone.getObjects()
        if #objs > 1 then
            local deck = public_zone.getObjects()[2]
            if deck.is_face_down then
                return true
            end
        end
        return false
    end
    local toRunFunc = function()
        local deck = public_zone.getObjects()[2]
        for i=1, 5 do
            local shop_zone = getObjectFromGUID(Global.getTable('shop_zone')[i])
            local params = {
                position=shop_zone.getPosition(),
                rotation={0, 0, 0},
                top=true,
            }
            deck.takeObject(params)
        end
    end
    Wait.condition(toRunFunc, watchFunc, timeout)
end

function dispatch_hero(selected_style)
    local heros = {getObjectFromGUID(Global.getVar('frog'))}
    for k, zone_guid in ipairs(selected_style) do
        for k, card_guid in ipairs(Global.getTable('hero_table')) do
            local card_obj = getObjectFromGUID(card_guid)
            if not card_obj then
                print('Heros card not found, maybe in a deck.')
                return
            end
            local basic_info = Global.call('getCardBasicInfo', card_obj)
            if basic_info.card_info.zone_guid == zone_guid then
                table.insert(heros, card_obj)
                break
            end
        end
    end
    local players = getSeatedPlayers()
    for i, color in ipairs(players) do
        local drop_zone = getObjectFromGUID(Global.getTable('drop_zone')[i])
        for idx=6-i*2, 5-i*2, -1 do
            local card_obj = table.remove(heros, math.random(1, idx))
            local x, y, z = table.unpack(drop_zone.getPosition())
            if idx % 2 == 0 then
                x = x + 2
            else
                x = x - 2
            end
            card_obj.setPosition({x, y, z})
        end
    end
end

function click_func(obj, color, alt_click)
    if (color == 'White' or color == 'Grey') then
        print('Please select a player color.')
        return
    end
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))
    if (#public_zone.getObjects() > 1) then
        print('Player already put deck on public card pile. Just place card.')
        self.editButton({index=0, tooltip='在商店放置5张牌.'})
        put_card_to_shop()
        return
    else
        self.editButton({index=0, tooltip='随机选择三种风格快速游戏'})
    end
    remain_pile_num = 0
    for i=1,6 do
        local pile = getObjectFromGUID(common_decks[i])
        if pile.getQuantity() > 0 then
            remain_pile_num = remain_pile_num + 1
        end
    end
    if remain_pile_num < 3 then
        print('Remain card pile less then 3.')
        return
    end

    local selected_style = move_card_pile_to_public()
    put_card_to_shop()
    dispatch_hero(selected_style)
end
