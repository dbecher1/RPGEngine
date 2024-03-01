
## TO DO:

- Due for a project-wide code cleanup too.
  - Definitely could benefit from adding more comments too.
- Fix entity animation bug - animation does not begin if state doesn't change
- Revisit the big picture plan for the structure of these intertwined subsystems
    - Sit down and think about how exactly I want to structure the interactions/organization between SceneManager/EntityManager/Scenes/ResourceManager. Something feels redundant.
- Separate the player controllable character from the sprite representation itself. This will make the system conform much easier to the intended vision
- With basic UI is done, refine:
  - Add in toggles, sub-menus, etc
  - Implement UI update - add size-agnostic features
  - Look into skinning?? Might be hard with SDL. Could do some alpha masking hackery though.
    - Alternatively, maybe generate them at runtime as a texture? That's possible and could build off the code already there, just will likely be a lot of work still.
 - Rectify resizing with a camera
 - Implement camera update, make sub-square and smooth out movement
    - Guts are there, gotta revisit it
    - Try to implement culling as well
 - Audio engine (basics)

### Longer term:

 - Flesh out scenes, add scene loader to resource manager
 - Character details (stats, etc) and supporting structure
 - Battle scene basics
 - Create something that's relatively playable
 - Flesh out input manager

## Done:
 - Basic UI
 - Entity loader, revision 2
 - Spritebatch
   - Performance issues fixed
 - Basic environment/map loading
 - Basic animation support
 - Basic input manager