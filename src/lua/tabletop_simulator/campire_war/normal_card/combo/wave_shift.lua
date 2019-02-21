basic_info = {
    type='card',
    card_info = {
        name='轮摆式移位',
        type='martial',
        style='combo',
        value=8,
        resource={
            power=2,
        },
        effect={
            order={'discard_to_card', 'shuffle', 'put'},
            discard_to_card=0,
            shuffle=true,
            put=7,
        },
        keyword={},
    },
}
