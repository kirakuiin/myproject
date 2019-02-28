basic_info = {
    type='card',
    card_info = {
        name='燃至灰烬',
        type='martial',
        style='fight',
        value=2,
        resource={
            power=1,
        },
        effect={
            order={'hand_calc', 'random_discard'},
            hand_calc={
                resource={
                    power=1,
                },
            },
            random_discard=0,
        },
        keyword={},
    },
}
