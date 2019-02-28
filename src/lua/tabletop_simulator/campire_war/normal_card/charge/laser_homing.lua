basic_info = {
    type='card',
    card_info = {
        name='激光制导',
        type='spell',
        style='charge',
        value=3,
        resource={},
        effect={
            exile_hand=true,
            resource={
                fire=1,
            },
            card_check={
                name='营火',
                resource={
                    fire=1,
                },
                card='next_exile_card'
            },
        },
        keyword={},
    },
}
