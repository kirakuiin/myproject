basic_info = {
    type='card',
    card_info = {
        name='粒子数反转',
        type='spell',
        style='charge',
        value=4,
        resource={},
        effect={
            order={'drop_effect', 'type_check'},
            drop_effect={
                discount=1,
                object={'self'},
            },
            type_check={
                card='next_discard_card',
                order={'type', 'resource'},
                type='spell',
                resource={
                    power=2,
                },
            },
        },
        keyword={
            exile={
                resource={
                    fire=1,
                },
            },
        },
    },
}
