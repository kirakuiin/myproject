basic_info = {
    type='card',
    card_info = {
        name='力量检定',
        type='martial',
        style='spirit',
        value=4,
        resource={},
        effect={
            order={'show', 'resource', 'value_check'},
            show=1,
            resource={
                power=2,
            },
            value_check={
                card='pile_top_card',
                ge_2={
                    power=1,
                },
            },
        },
        keyword={},
    },
}
