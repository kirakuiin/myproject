卡牌关键字记录
resource:打出牌获得的资源
effect:打出时获得的效果
keyword:关键字, 效果发生时间由关键字决定

resource = {
    fire[int]: 营火数
    life[int]: 生命数
    power[int]: 战力数
}

effect = {
    order[table]: 效果执行顺序
    show[bool]: 展示牌库顶牌
    put[int]: 在牌桌上放置n张牌
    draw[int]: 抓n张牌
    discard[int]: 弃n张牌
    discard_to_card[int]: 移动n张弃牌到牌库堆, n==0代表所有
    discard_to_hand[table]: 弃牌区到手牌
    shuffle[bool]: 洗牌
    card_check[table]: 卡牌检定
    value_check[table]: 费用检定
    style_check[table]: 风格检定
    value_calc[table]: 费用数计算
    style_calc[table]: 风格数计算
    type_calc[table]: 类型计算
    hand_calc[table]: 计算手牌数
    exile_hand[bool]: 放逐手牌
    exile_drop[bool]: 放逐打出区
    exile_self_zone[bool]: 放逐打出区或弃牌区
    exile_public[int]: 放逐公共区牌
    loss_lp[table]: 某对象失去n生命
    random_discard[table]: 某对象随机弃牌n张, n==0 代表所有
    discount[int]: 购买时折扣
    type_check[table]: 类型检定
    equip[bool]: 直接装备
    put_on_top[bool]: 放到牌库顶
    drop_effect[bool]: 被动效果
    ======================================
    charge_and_fire[bool]: 佩里夫特效果
    play_is_life[bool]: 冠军腰带效果
    brick[bool]: 板砖效果
    liver[bool]: 肝脏打击效果
    double_spell[bool]: 谐振腔效果
    first_invalid[bool]: 致盲射线效果
    replay[bool]: 暗影步效果
    execute_exile[bool]: 噩梦化身效果
    power_is_money[bool]: 超重量级武器效果
    rise[bool]: 起势效果
    exchange[bool]: 震空波动拳效果
    double_resource[bool]: 电眼逼人效果
    rage[bool]: 怒气燃烧效果
    exile_lp_modify[int]: 营地蛙
}

keyword = {
    probe[n]: 探查n张牌
    gain[table]: 获得时
    disadv[table]: 背水
    charge[table]: 充能
    air[table]: 聚气
    exile[table]: 放逐
    combo[table]: 连击
    guard[bool]: 护卫
}
