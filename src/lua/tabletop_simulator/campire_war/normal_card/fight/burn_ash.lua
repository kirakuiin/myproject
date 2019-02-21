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
            order={'hand_calc', 'discard'},
            hand_calc={
                resource={
                    power=1,
                },
            },
            discard=0,
        },
        keyword={},
    },
}
