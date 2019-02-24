basic_info = {
    type='card',
    card_info = {
        name='冠军腰带',
        type='item',
        style='combo',
        value=7,
        defense=3,
        resource={},
        effect={
            drop_effect={
                play_is_life=true,
                object={'self'},
            },
        },
        keyword={
            gain={
                type_calc={
                    type='martial',
                    max=5,
                    resource={
                        life=1,
                    }
                },
            },
        },
    },
}
