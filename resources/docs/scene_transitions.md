
### Rough flowchart of how I want things like scene transitions to go
Need to organize my thoughts somewhere

Overworld -> Battle
- Battle trigger happens, scene manager notified (maybe make my own event poll?)
- Scene manager swaps global state?
- Enemy type is selected (somehow)
- New battle is instantiated

Problems:
- Global state is still suspect
- How do we store party data?
  - I don't like it, but maybe global state still?
- What selects enemy data? Is that the scene, because of the map?