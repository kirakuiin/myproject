-- author:wang.zhuowei@foxmail.com
-- date: Feb 8, 2019 -- license: GPL.v3
--[[ Global zone guid --]]

public_card_pile = '010589'
campire_card_pile = '9110b6'
shop_slot_1 = '6372a8'
shop_slot_2 = 'b22d01'
shop_slot_3 = 'dea759'
shop_slot_4 = '4c53f7'
shop_slot_5 = 'ce5c67'
public_exile = 'a13366'

player_show_zone_1 = 'd2aa44'
player_card_pile_1 = 'c7d826'
player_drop_zone_1 = '0a6cc0'
player_discard_pile_1 = '776f30'
player_exile_pile_1 = '53e9df'
player_character_zone_1 = '9efac3'
player_item_zone_one_1 = '4756a7'
player_item_zone_two_1 = '602a7d'
player_hand_zone_1 = '1629b4'
player_dice6_1 = '64073e'

player_show_zone_2 = 'c4a79b'
player_card_pile_2 = '7359e7'
player_drop_zone_2 = 'dd6fb5'
player_discard_pile_2 = 'b09018'
player_exile_pile_2 = 'b31559'
player_character_zone_2 = '8a29d9'
player_item_zone_one_2 = 'b78edb'
player_item_zone_two_2 = 'c99fae'
player_hand_zone_2 = '1d9376'
player_dice6_2 = 'e9404d'

--[[ Global object guid --]]

board = '6f37e7'
quick_start_token = 'd1937e'
test_token = '853784'
disable_token = '77fb4b'

qigong_bag = '1d2a03'
combo_bag = 'fcde40'
mix_bag = 'a26060'
spirit_bag = '293ca0'
fight_bag = '363384'
charge_bag = '12b623'

combo_select_token = 'a5b299'
spirit_select_token = 'fba955'
mix_select_token = 'd4085f'
fight_select_token = '97d15d'
charge_select_token = '659459'
qigongz_select_token = 'ab6ef3'

first_hand_token_1 = '12997a'
turn_end_token_1 = 'b99696'
power_token_1 = '43de29'
fire_token_1 = 'fb325f'
lp_token_1 = 'ac4036'

first_hand_token_2 = '6cad06'
turn_end_token_2 = '0f21bf'
power_token_2 = '688c09'
fire_token_2 = 'e423d3'
lp_token_2 = '54ec57'

--[[ Some set --]]

shop_zone = {
    shop_slot_1, shop_slot_2, shop_slot_3, shop_slot_4, shop_slot_5
}

item_zone = {
    player_item_zone_one_1, player_item_zone_one_2,
    player_item_zone_two_1, player_item_zone_two_2,
}

drop_zone = {
    player_drop_zone_1, player_drop_zone_2,
}

card_zone = {
    player_card_pile_1, player_card_pile_2
}

player_zone = {player_character_zone_1, player_character_zone_2}

common_decks = {
    qigong_bag, combo_bag, mix_bag, spirit_bag, fight_bag, charge_bag,
}

player1_obj = {
    player_show_zone=player_show_zone_1,
    player_card_pile=player_card_pile_1,
    player_drop_zone=player_drop_zone_1,
    player_discard_pile=player_discard_pile_1,
    player_exile_pile=player_exile_pile_1,
    player_character_zone=player_character_zone_1,
    player_item_zone_one=player_item_zone_one_1,
    player_item_zone_two=player_item_zone_two_1,
    player_hand_zone=player_hand_zone_1,
    first_hand_token=first_hand_token_1,
    turn_end_token=turn_end_token_1,
    power_token=power_token_1,
    fire_token=fire_token_1,
    lp_token=lp_token_1,
    player_dice6=player_dice6_1,
}

player2_obj = {
    player_show_zone=player_show_zone_2,
    player_card_pile=player_card_pile_2,
    player_drop_zone=player_drop_zone_2,
    player_discard_pile=player_discard_pile_2,
    player_exile_pile=player_exile_pile_2,
    player_character_zone=player_character_zone_2,
    player_item_zone_one=player_item_zone_one_2,
    player_item_zone_two=player_item_zone_two_2,
    player_hand_zone=player_hand_zone_2,
    first_hand_token=first_hand_token_2,
    turn_end_token=turn_end_token_2,
    power_token=power_token_2,
    fire_token=fire_token_2,
    lp_token=lp_token_2,
    player_dice6=player_dice6_2,
}

--[[ Constants --]]

timeout = 10
show_timeout = 3
wait_timeout = 0.5

White = {r=1, g=1, b=1}
Brown = {r=0.443, g=0.231, b=0.09}
Red = {r=0.856, g=0.1, b=0.094}
Orange = {r=0.956, g=0.392, b=0.113}
Yellow = {r=0.905, g=0.898, b=0.172}
Green = {r=0.192, g=0.701, b=0.168}
Teal = {r=0.129, g=0.694, b=0.607}
Blue = {r=0.118, g=0.118, b=0.531}
Purple = {r=0.627, g=0.125, b=0.941}
Pink = {r=0.96, g=0.439, b=0.807}
Grey = {r=0.5, g=0.5, b=0.5}
Black = {r=0.25, g=0.25, b=0.25}

fire_url = 'http://m.qpic.cn/psb?/V13semAK0lQLYQ/yO89qBaZXt16oqPVcPUogzV*QPfqp5VMEeuyvhVdN1Y!/b/dL4AAAAAAAAA&bo=CAEhAQgBIQEDGTw!&rf=viewer_4'
power_url = 'http://m.qpic.cn/psb?/V13semAK0lQLYQ/fuoIIaZDYAVO3snDQEGAxRyzg5wpiKsY3Qhb0FOCmbA!/b/dL8AAAAAAAAA&bo=HgEpAR4BKQEDGTw!&rf=viewer_4'
shield_url = 'http://m.qpic.cn/psb?/V13semAK0lQLYQ/2dX9brVN7tJUmyg96ZxPdO56dN5BH8RAlXztJw0ORnc!/b/dFMBAAAAAAAA&bo=kAGKAQAAAAADFyg!&rf=viewer_4'
launch_url = 'http://m.qpic.cn/psb?/V13semAK0lQLYQ/4Vf4ZglQbbqodUxCwNmakVag4SReXG2wAFyDHH8qfH8!/b/dFQBAAAAAAAA&bo=kAGYAQAAAAADByo!&rf=viewer_4'

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
    if obj then
        return obj.tag == 'Card'
    end
end

function isDeck(obj)
    if obj then
        return obj.tag == 'Deck'
    end
end

function getCardInfo(card_obj)
    if card_obj then
        return getCardBasicInfo(card_obj).card_info
    end
    return nil
end

function isValidColor(color)
    if (color == 'White' or color == 'Grey' or color == 'Black' or
        color == nil) then
        return false
    else
        return true
    end
end

function printTable(tab_obj)
    if type(tab_obj) == 'table' then
        print('{')
        for k, v in pairs(tab_obj) do
            if type(v) == 'table' then
                print('key=', k, ' v=')
                printTable(v)
            else
                print('key=', k, ' v=', v)
            end
        end
        print('}')
    else
        print('Not a table!')
    end
end

function clone(object)
    local lookup_table = {}
    local function copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[copy(key)] = copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return copy(object)
end

function hashConvert(v)
    local ch = 0
    local val = 0
    if(v) then
        for i=1,#v do
            ch = v:byte(i)
            if( ch >= 65 and ch <= 90 ) then
                ch = ch + 32
            end
            val = val*0.7 + ch  --0.7是加权
        end
    end
    val = val .. ''
    val = val:gsub("+","")
    val = val:gsub("%.","")
    return string.format('%s',val)
end

function getCardBasicInfo(obj)
    if obj then
        return obj.getTable('basic_info')
    end
    return nil
end

function isNonItemCard(obj)
    if isCard(obj) then
        local basic_info = getCardBasicInfo(obj)
        if (basic_info.type == 'card') and
            (getCardInfo(obj).type ~= 'item') then
            return true
        end
    end
    return false
end

function isItemCard(obj)
    if isCard(obj) then
        local basic_info = getCardBasicInfo(obj)
        if (basic_info.type == 'card') and
            (getCardInfo(obj).type == 'item') then
            return true
        end
    end
    return false
end

function isHeroCard(obj)
    if isCard(obj) then
        local basic_info = getCardBasicInfo(obj)
        if basic_info.type == 'hero' then
            return true
        end
    end
    return false
end

function getColorsObjs(color)
    local hand_z = Player[color].getHandTransform().position.z
    if hand_z < 0 then
        return player1_obj
    else
        return player2_obj
    end
end

function getAnotherColorsObjs(color)
    local hand_z = Player[color].getHandTransform().position.z
    if hand_z < 0 then
        return player2_obj
    else
        return player1_obj
    end
end

function getObjsColor(obj)
    for i, color in ipairs(getSeatedPlayers()) do
        if isValidColor(color) then
            local objs = getColorsObjs(color)
            for _, obj_guid in pairs(objs) do
                if obj_guid == obj.guid then
                    return color
                end
            end
        end
    end
    if obj.getPosition().z < 0 then
        return 'Blue'
    else
        return 'Red'
    end
end

function getAnotherColor(color)
    if isValidColor(color) then
        local objs = getAnotherColorsObjs(color)
        return getObjsColor(getObjectFromGUID(objs.lp_token))
    end
    return 'White'
end

function getResourceInfo(resource_name)
    local color = nil
    local tooltip = nil
    local setfunc = nil
    local getfunc = nil
    if resource_name == 'fire' or resource_name == '营火' then
        color = 'Orange'
        tooltip = '营火'
        setfunc = 'setPlayerFire'
        getfunc = 'getPlayerFire'
    elseif resource_name == 'life' or resource_name == '生命' then
        color = 'Yellow'
        tooltip = '生命'
        setfunc = 'setPlayerLp'
        getfunc = 'getPlayerLp'
    elseif resource_name == 'power' or resource_name == '战力' then
        color = 'Red'
        tooltip = '战力'
        setfunc = 'setPlayerPower'
        getfunc = 'getPlayerPower'
    end
    return {color=color, tooltip=tooltip, setfunc=setfunc, getfunc=getfunc}
end

function getAboveZoneObj(obj)
    if #obj.getObjects() == 2 then
        for _, a_obj in ipairs(obj.getObjects()) do
            if a_obj.guid ~= board then
                return a_obj
            end
        end
    end
    return nil
end

function getAboveZoneObjs(obj)
    local objs_table = {}
    for _, a_obj in ipairs(obj.getObjects()) do
        if a_obj.guid ~= board then
            table.insert(objs_table, a_obj)
        end
    end
    if #objs_table > 0 then
        return objs_table
    else
        return nil
    end
end

--[[ obj1: first obj, obj2: second obj --]]
function isCloseEnough(params)
    local pos1= params.obj1.getPosition()
    local pos2= params.obj2.getPosition()
    local err = params.error
    if not err then
        err = 0.15
    end
    if (math.abs(pos1.x-pos2.x) > err) or (math.abs(pos1.z-pos2.z) > err) then
        return false
    end
    return true
end

function getZoneCardNum(params)
    local color = params.color
    local zone = params.zone
    local objs = getAboveZoneObjs(zone)
    local total = 0
    if objs then
        for _, obj in pairs(objs) do
            if isCard(obj) or isDeck(obj) then
                local tmp = obj.getQuantity()
                if tmp == -1 then tmp = 1 end
                total = total + tmp
            end
        end
    end
    return total
end

function getCardPileNum(color)
    local zone = getObjectFromGUID(getColorsObjs(color).player_card_pile)
    return getZoneCardNum({color=color, zone=zone})
end

--[[ color: player_color, src: src_zone, dest: dest_zone, ready: return value --]]
--[[ flip: is flip object, quick: is instant move, no_shuffle: not shuffle --]]
function moveZoneToZone(params)
    local src = params.src
    local dest = params.dest
    local objs = getAboveZoneObjs(src)
    local card_objs = getAboveZoneObjs(dest)
    local color = params.color
    local src_num = getZoneCardNum({zone=src, color=color})
    local dest_num = getZoneCardNum({zone=dest, color=color})
    local dest_rot = {0, 180, 0}

    local flip_done = false
    if card_objs then
        for _, card_obj in pairs(card_objs) do
            if card_obj.is_face_down then
                card_obj.flip()
            end
        end
        local watchFunc = function()
            for _, card_obj in pairs(card_objs) do
                if card_obj.is_face_down then return false end
            end
            return true
        end
        local toRunFunc = function()
            Wait.time(function() flip_done = true end, wait_timeout)
        end
        Wait.condition(toRunFunc, watchFunc, timeout)
    else
        flip_done = true
    end

    if objs then
        local watchFunc = function() return flip_done end
        local toRunFunc = function()
            for _, obj in ipairs(objs) do
                if isCard(obj) or isDeck(obj) then
                    local dest_pos = dest.getPosition()
                    if dest_pos.z > 0 then dest_rot = {0, 360, 0} end
                    if params.quick then
                        obj.setPosition(dest_pos)
                        obj.setRotation(dest_rot)
                    else
                        obj.setPositionSmooth(dest_pos, false, true)
                        obj.setRotationSmooth(dest_rot, false, true)
                    end
                end
            end
        end
        Wait.condition(toRunFunc, watchFunc, timeout)
    else
        params.ready = true
        return src_num
    end

    local watchFunc = function()
        local dest_objs = getAboveZoneObjs(dest)
        if dest_objs then
            if (src == dest) and
               isCloseEnough({obj1=dest, obj2=dest_objs[1]}) then
                    return true
            end
            if #dest_objs == 1 then
                local obj_num = getZoneCardNum({color=color, zone=dest})
                -- make sure all card in zone
                if (obj_num == src_num + dest_num) and
                    isCloseEnough({obj1=dest, obj2=dest_objs[1], err=0.5}) then
                    return true
                end
            end
        end
        return false
    end
    local toRunFunc = function()
        local dest_obj = getAboveZoneObj(dest)
        if not params.no_shuffle then
            dest_obj.shuffle()
        end
        dest_obj.setRotationSmooth(dest_rot, false, true)
        if params.flip then
            dest_obj.flip()
            local watchFunc = function()
                if not dest_obj.is_face_down then
                    return false
                else
                    return true
                end
            end
            local toRunFunc = function() params.ready = true end
            Wait.condition(toRunFunc, watchFunc, timeout,
                           function() print('flip timeout') end)
        else
            params.ready = true
        end
    end
    Wait.condition(toRunFunc, watchFunc, timeout,
                   function() print('move timeout') end)
    return src_num
end

function moveDiscardToCard(params)
    local objs = getColorsObjs(params.color)
    local discard = getObjectFromGUID(objs.player_discard_pile)
    local card = getObjectFromGUID(objs.player_card_pile)
    params.src = discard
    params.dest = card
    params.flip = true
    return moveZoneToZone(params)
end

function moveDropToDiscard(params)
    local objs = getColorsObjs(params.color)
    local drop = getObjectFromGUID(objs.player_drop_zone)
    local discard = getObjectFromGUID(objs.player_discard_pile)
    params.src = drop
    params.dest = discard
    return moveZoneToZone(params)
end

function moveHandToDiscard(params)
    local objs = getColorsObjs(params.color)
    local hand = getObjectFromGUID(objs.player_hand_zone)
    local discard = getObjectFromGUID(objs.player_discard_pile)
    params.src = hand
    params.dest = discard
    params.quick = true
    moveZoneToZone(params)
end

--[[ guid: item zone guid --]]
function moveItemToDiscard(params)
    local objs = getColorsObjs(params.color)
    local item = getObjectFromGUID(params.guid)
    local discard = getObjectFromGUID(objs.player_discard_pile)
    params.src = item
    params.dest = discard
    params.quick = true
    params.no_shuffle = true
    return moveZoneToZone(params)
end

--[[ color: player's color, num: draw number --]]
function drawCard(params)
    local objs = getColorsObjs(params.color)
    local card_pile_zone = getObjectFromGUID(objs.player_card_pile)
    local deck_obj = getAboveZoneObj(card_pile_zone)
    local need_num = params.num
    if deck_obj then
        local deck_num = deck_obj.getQuantity()
        if deck_num < 0 then
            deck_num = 1
        end
        if deck_num < need_num then
            deck_obj.deal(deck_num, params.color)
            need_num = need_num - deck_num
        else
            deck_obj.deal(need_num, params.color)
            return
        end
    end
    local wFunc = function()
        local card_obj = getAboveZoneObjs(card_pile_zone)
        if not card_obj then
            return true
        end
        return false
    end
    local rFunc = function()
        params.ready = false
        local new_deck_num = moveDiscardToCard(params)
        if new_deck_num > 0 then
            local watchFunc = function() return params.ready end
            local toRunFunc = function()
                local new_deck = getAboveZoneObj(card_pile_zone)
                Wait.time(function() new_deck.deal(need_num, params.color) end,
                          wait_timeout)
            end
            Wait.condition(toRunFunc, watchFunc, timeout)
        end
    end
    Wait.condition(rFunc, wFunc, timeout)
end

function removeButton(btn_obj)
    if btn_obj.getButtons() then
        for _, btn in pairs(btn_obj.getButtons()) do
            btn_obj.removeButton(btn.index)
        end
    end
end

function removeInput(input_obj)
    if input_obj.getInputs() then
        for _, input in pairs(input_obj.getInputs()) do
            input_obj.removeInput(input.index)
        end
    end
end

function removeAllUI(obj)
    removeInput(obj)
    removeButton(obj)
end

--[[ Player manager func --]]

function turnEndDropAndDraw(color)
    local param = {color=color, ready=false}
    moveHandToDiscard(param)
    local watchFunc = function() return param.ready end
    local toRunFunc = function()
        param.ready = false
        moveDropToDiscard(param)
        local watchFunc = function() return param.ready end
        local toRunFunc = function() drawCard({color=color, num=5}) end
        Wait.condition(toRunFunc, watchFunc, timeout)
    end
    Wait.condition(toRunFunc, watchFunc, timeout)
end

function getPlayerLp(color)
    local lp_obj = getObjectFromGUID(getColorsObjs(color).lp_token)
    return tonumber(lp_obj.getButtons()[1].label)
end
function setPlayerLp(params)
    local lp_obj = getObjectFromGUID(getColorsObjs(params.color).lp_token)
    lp_obj.editButton({index=0, label=tostring(params.value)})
end

function getPlayerFire(color)
    local lp_obj = getObjectFromGUID(getColorsObjs(color).fire_token)
    return tonumber(lp_obj.getButtons()[1].label)
end
function setPlayerFire(params)
    local lp_obj = getObjectFromGUID(getColorsObjs(params.color).fire_token)
    lp_obj.editButton({index=0, label=tostring(params.value)})
end

function getPlayerPower(color)
    local lp_obj = getObjectFromGUID(getColorsObjs(color).power_token)
    return tonumber(lp_obj.getButtons()[1].label)
end
function setPlayerPower(params)
    local lp_obj = getObjectFromGUID(getColorsObjs(params.color).power_token)
    lp_obj.editButton({index=0, label=tostring(params.value)})
end

function show(color)
    local card_num = getCardPileNum(color)
    local card_pile = getObjectFromGUID(getColorsObjs(color).player_card_pile)
    local deck = getAboveZoneObj(card_pile)
    local card = nil
    if card_num > 1 then
        card = deck.takeObject({
            position=card_pile.getPosition(),
            flip=true,
        })
    elseif card_num == 1 then
        card = deck
        deck.flip()
    else
        return
    end
    Wait.time(function() card.flip() end, show_timeout)
end

function probe(color, num)
    local card_num = getCardPileNum(color)
    if card_num >= num then
        drawCard({color=color, num=num})
    elseif card_num ~= 0 then
        drawCard({color=color, num=card_num})
    end
end

function shuffle(color)
    local color = color
    local card_num = getCardPileNum(color)
    local card_pile = getObjectFromGUID(getColorsObjs(color).player_card_pile)
    local deck = getAboveZoneObj(card_pile)
    if deck then
        deck.shuffle()
    end
end

function put(color, num)
    local objs = getColorsObjs(color)
    local card = getObjectFromGUID(objs.player_card_pile)
    local show = getObjectFromGUID(objs.player_show_zone)
    local card_num = getCardPileNum(color)
    local deck = getAboveZoneObj(card)
    local put_num = num
    if put_num > card_num then put_num = card_num end
    local x, y, z = table.unpack(show.getPosition())
    if card_num == 1 then
        deck.flip()
        deck.setPositionSmooth({x-3, y, z}, false, true)
    elseif card_num > 1 then
        for x_base = -3,3 do
            deck.takeObject({
                position={x_base+x, y, z},
                flip=true,
            })
            put_num = put_num - 1
            if put_num == 0 then break end
        end
    end
end

function randomDiscard(color, num)
    local objs = Global.call('getColorsObjs', color)
    local hand = getObjectFromGUID(objs.player_hand_zone)
    local discard = getObjectFromGUID(objs.player_discard_pile)
    local all_card = getAboveZoneObjs(hand)
    local discard_num = num
    if all_card then
        local card_num = #all_card
        if card_num < discard_num then
            discard_num = card_num
        end
        for i=1,discard_num do
            local index = math.random(1, card_num)
            local obj = table.remove(all_card, index)
            obj.setPosition(discard.getPosition())
            card_num = card_num - 1
        end
    end
end

function isCardOp(name)
    for _, v in pairs({'draw', 'show', 'probe', 'put', 'random_discard',
                       'shuffle', 'discard_to_card'}) do
        if name == v then
            return true
        end
    end
end

--[[ name: effect name, color: player color, arg: effect arguments ]]
function executeCardOp(params)
    local color = params.color
    local key = params.key
    local arg = params.arg
    local result = false
    if key == 'draw' then
        result = drawCard({color=color, num=arg})
    elseif key == 'show' then
        result = show(color)
    elseif key == 'probe' then
        result = probe(color, arg)
    elseif key == 'discard_to_card' and arg == 0 then
        result = moveDiscardToCard({color=color})
    elseif key == 'shuffle' then
        result = shuffle(color)
    elseif key == 'random_discard' then
        result = randomDiscard(color, arg)
    elseif key == 'put' then
        result = put(color, arg)
    end
    return result
end

--[[ Global save var --]]

selected_style = {}
function addNewStyle(guid)
    table.insert(selected_style, guid)
end
function getStyle()
    return selected_style
end

disable = false
function disableAllScript()
    disable = true
end
function enableAllScript()
    disable = false
end
function isDisable()
    return disable
end

picking_obj = {color=nil, obj=nil}
function getPickingInfo()
    return picking_obj
end

game_info = {}
function setGameInfo(params)
    for k, v in pairs(params) do
        game_info[k] = v
    end
end
function getGameInfo()
    return game_info
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
end

--[[ The onUpdate event is called once per frame. --]]
function onUpdate()
    --[[ print('onUpdate loop!') --]]
end

-- [[ Called whenever a Player picks up an Object. --]]
function onObjectPickUp(color, obj)
    picking_obj['color'] = color
    picking_obj['obj'] = obj
end
