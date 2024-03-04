
# Simple RPG Engine (very early WIP)

The short of things is that there's a popular 3D RPG I'd like to make a "demake" of, so I tried to learn RPG Maker to do it. Hated it. Downloaded Unity, and while learning the initial goings ons of it I had the idea of just building it myself from scratch, using as few third party libraries as I can.

So that's what I'm doing. Started this project mid-February '24, and as of writing this (March 4th) I'm happy with the work 2ish weeks has produced. Some notes of what I'm proud of:

- My Resource Manager is fully functioning with the resources I've worked with so far. All loaded sprites are made into one single atlas at runtime, minimizing context switches for the rendering backend.
- My sprite rendering is mostly online with 9 Z-layers and Y-sorting for the middle 3 layers, animations work well and are fairly configurable, but can be expanded upon. Effects are (eventually) next which will require tinkering because I'm using mostly raw SDL2.
- I built my own text renderer off the back of my sprite renderer
- Built what's essentially the start of an immediate mode GUI for the in-game interface using SDL primitives and the power of math. Furthermore, I figured out how to skin the UI with a texture as well.

I've been keeping a full rolling to do list in the appropriately named document, this is just a few bullet points. Thanks for reading!