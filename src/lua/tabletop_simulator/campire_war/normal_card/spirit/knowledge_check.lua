basic_info = {
    type='card',
    card_info = {
        name='知识检定',
        type='martial',
        style='spirit',
        value=3,
        resource={},
        effect={
            order={'value_check', 'put', 'draw'},
            put=1,
            draw=1,
            value_check={
                card='pile_top_card',
                l_2={
                    loss_lp={
                        object={'self'},
                        value=2,
                    },
                },
            },
        },
        keyword={},
    },
}
