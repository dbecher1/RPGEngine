
## TO DO:
- FIX THE BATTLE SCENE:( I broke it by using window coordinates....
- Revisit the big picture plan for the structure of these intertwined subsystems
    - Sit down and think about how exactly I want to structure the interactions/organization between SceneManager/EntityManager/Scenes/ResourceManager. Something feels redundant.
- Separate the player controllable character from the sprite representation itself. This will make the system conform much easier to the intended vision
- With basic UI is done, refine:
  - Implement loading UI models from data
  - Experiment with using SDL_RenderGeometry to create higher fidelity borders 
  - Text word wrapping
  - Add in toggles, sub-menus, etc
  - Implement UI update method
 - Implement camera update, make sub-square and smooth out movement
    - Guts are there, gotta revisit it
    - Try to implement culling as well
    - Rectify resizing with a camera
 - Audio engine (basics)
 - Integrate Dear IMGUI into the engine for much easier debugging

### Longer term:

 - Flesh out scenes, add scene loader to resource manager
 - Character details (stats, etc) and supporting structure
 - Battle scene basics
 - Create something that's relatively playable
 - Flesh out input manager
 - Definitely could benefit from adding more comments too.

## Done:
 - Entity loader, revision 2
 - Spritebatch
   - Performance issues fixed
 - Basic environment/map loading
 - Basic animation support
 - Basic input manager
 - Fix entity animation bug - animation does not begin if state doesn't change
 - Basic UI
 - Basic UI skinning?? Whaaaat
 - Font loading and glyph rendering!!