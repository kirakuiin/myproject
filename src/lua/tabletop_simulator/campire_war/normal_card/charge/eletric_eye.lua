basic_info = {
    type='card',
    card_info = {
        name='电眼逼人',
        type='spell',
        style='charge',
        value=8,
        resource={},
        effect={
            order={'draw', 'discard', 'double_resource'},
            draw=3,
            discard=3,
            double_resource={
                type='or',
                fire_2=true,
                power_2=true,
            },
        },
        keyword={},
    },
}
