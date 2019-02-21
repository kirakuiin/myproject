basic_info = {
    type='card',
    card_info = {
        name='乌鸦坐飞机',
        type='martial',
        style='fight',
        value=3,
        resource={},
        effect={
            order={'random_discard', 'resource'},
            random_discard=1,
            resource={
                power=2,
            },
        },
        keyword={
            exile={
                resource={
                    life=1,
                },
            },
            disadv={
                resource={
                    power=1,
                },
            },
        },
    },
}
