basic_info = {
    type='card',
    card_info = {
        name='灵感检定',
        type='martial',
        style='spirit',
        value=2,
        resource={},
        effect={
            order={'value_check', 'put'},
            put=1,
            value_check={
                card='pile_top_card',
                l_2={
                    resource={
                        fire=1,
                    },
                },
            },
        },
        keyword={},
    },
}
