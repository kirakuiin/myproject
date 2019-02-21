--[[ This is quick start script for campfire war card game. --]]
-- author:wang.zhuowei@foxmail.com
-- date: Feb 11, 2019
-- license: GPL.v3

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    print('Start button onLoad!')
    common_decks = Global.getTable('common_decks')

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
            Global.call('addNewStyle', common_decks[num])
        end
    end

    local watchFunc = function()
        local deck = Global.call('getAboveZoneObj', public_zone)
        if deck then
            if deck.getQuantity() == 60 then
                return true
            end
        end
        return false
    end
    local toRunFunc = function()
        local deck = Global.call('getAboveZoneObj', public_zone)
        deck.flip()
        deck.shuffle()
    end
    Wait.condition(toRunFunc, watchFunc, Global.getVar('timeout'))
end

function put_card_to_shop()
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))

    local watchFunc = function()
        local deck = Global.call('getAboveZoneObj', public_zone)
        if deck then
            if deck.is_face_down then
                return true
            end
        end
        return false
    end
    local toRunFunc = function()
        local deck = Global.call('getAboveZoneObj', public_zone)
        deck.shuffle()
        for i=1, 5 do
            local shop_zone = getObjectFromGUID(Global.getTable('shop_zone')[i])
            local params = {
                position=shop_zone.getPosition(),
                rotation={0, 0, 0},
                top=true,
                smooth=false,
            }
            if #shop_zone.getObjects() == 1 then
                deck.takeObject(params)
            end
        end
    end
    Wait.condition(toRunFunc, watchFunc, Global.getVar('timeout'))
end

function dispatch_hero()
    local selected_style = Global.call('getStyle')
    local heros = {getObjectFromGUID(Global.getVar('frog'))}
    for k, zone_guid in ipairs(selected_style) do
        for k, card_guid in ipairs(Global.getTable('hero_table')) do
            local card_obj = getObjectFromGUID(card_guid)
            if not card_obj then
                broadcastToAll('无法找到英雄卡, 也许在牌组里',
                               Global.getTable('Purple'))
                return
            end
            local basic_info = Global.call('getCardBasicInfo', card_obj)
            if basic_info.card_info.zone_guid == zone_guid then
                table.insert(heros, card_obj)
                break
            end
        end
    end
    if #heros ~= 4 then
        broadcastToAll('选择风格不等于3, 无效!', Global.getTable('Purple'))
        return
    end
    local players = getSeatedPlayers()
    for i, color in ipairs(players) do
        local drop_zone = getObjectFromGUID(
            Global.call('getColorsObjs', color).player_drop_zone
        )
        for idx=6-i*2, 5-i*2, -1 do
            local card_obj = table.remove(heros, math.random(1, idx))
            local x, y, z = table.unpack(drop_zone.getPosition())
            if idx % 2 == 0 then
                x = x + 2
            else
                x = x - 2
            end
            card_obj.setPosition({x, y, z})
            if z > 0 then
                card_obj.setRotation({0, 360, 0})
            else
                card_obj.setRotation({0, 180, 0})
            end
        end
    end
end

function click_func(obj, color, alt_click)
    if Global.call('isDisable') then
        return
    end
    if (not Global.call('isValidColor', color)) then
        broadcastToAll('请选择一个玩家颜色.', Global.getTable('Purple'))
        return
    end
    local public_zone = getObjectFromGUID(Global.getVar('public_card_pile'))
    if (#public_zone.getObjects() == 1) then
        local remain_pile_num = 0
        for i=1,6 do
            local pile = getObjectFromGUID(common_decks[i])
            if pile.getQuantity() > 0 then
                remain_pile_num = remain_pile_num + 1
            end
        end
        if remain_pile_num < 3 then
            broadcastToAll('剩余可选英雄风格数小于3.',
                           Global.getTable('Purple'))
            return
        end
        move_card_pile_to_public()
    end
    put_card_to_shop()
    dispatch_hero()
end
