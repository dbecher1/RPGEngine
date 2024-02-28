# Entity layout:

Entities are comprised of two main components, with shared details between them.

- Shared:
    - Name
    - Elemental affinity (TODO)
    - Stats/Level/Progression
    - Is active (party order)
    - Abilities (TODO)
    - Status effects? If we take the Pokemon approach for things like poison

- Overworld component
    - Sprite
        - Obviously animation related details, size, etc here
    - Position
    - Party leader? (note: have the "tail" style party, or just leader?)
        - If leader obviously controls, collision, etc
    - Pull from shared stats for pause menu integration

- Battle component
    - Sprite
        - Pre-defined positions based on party composition
            - Different locations based on number of party members
            - A few options here
                - Only locations?
                - Front line / back line a la FFIX and others?
                - Surrounded/pincer attack a la FFVI? or standard only
                    - If we do this we have to work in backstabs into battle
                - Probably best to not have the entity worry about this...
    - Pull stats, abilities from shared component
    - Use speed stat for ATB
    - Biggest design decision that needs to be made: will a battle create a copy of the entity stats and then "write" to that after the battle? Or will it actively update the entity stats, so no special action (save for party member death, status effect, etc etc) needs to be taken upon scene exit

Active party will be stored as a state. Upon scene transition(ie overworld -> battle), no state changes need be triggered in the party. Entity objects can be queried regardless of scene and the result of that query will differ depending on the current (global? maybe a bad idea) state.

## TODO:
- Revisit the layout of the entity JSON object to incorporate the above