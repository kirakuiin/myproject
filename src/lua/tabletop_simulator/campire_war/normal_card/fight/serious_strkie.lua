basic_info = {
    type='card',
    card_info = {
        name='认真走脸',
        type='martial',
        style='fight',
        value=2,
        resource={},
        effect={
            order={'exile_hand', 'resource', 'card_check'},
            exile_hand=true,
            resource={
                power=1,
            },
            card_check={
                order={'name', 'resource'},
                card='next_exile_card',
                name='走脸',
                resource={
                    power=1,
                },
            },
        },
        keyword={},
    },
}
