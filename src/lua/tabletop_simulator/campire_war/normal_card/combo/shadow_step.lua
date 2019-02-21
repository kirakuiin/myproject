basic_info = {
    type='card',
    card_info = {
        name='暗影步',
        type='spell',
        style='combo',
        value=4,
        resource={},
        effect={
            value_check={
                card='prev_drop_card',
                le_4={
                    replay=true,
                },
            },
        },
        keyword={},
    },
}
