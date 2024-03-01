
## Scratch doc for events

# EventPoller - name WIP
Class that will essentially be a wrapper for a deque of events. Maybe a priority queue? But the event polling will function similarly to SDL's event polling

# Event
Struct representing an event
- Type
  - Datatype: Enum
    - Scene transition
      - If loading cutscenes, etc can couple that ID into the event
      - Can also pack in things like location data for battle transitions
    - Pause, UI changes
    - Things like merchants
- "Payload?"
  - Refining needed, but my rough idea is that there's some data that needs passing, and this can do that
    - If we want to get real stupid, we can play with void pointers. I shouldn't do that though.
    