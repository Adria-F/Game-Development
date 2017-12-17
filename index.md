# **Not A Typical Platformer** (N.A.T.P)
It is a platformer game done for the assignments of the videogames development subject in the UPC.
It consisted on 3 deliveries:
 1st - Creating the platformer itself, with a moving character and a special ability (double jump, dash, etc).
 2nd - Adding enemies with pathfinding (flying and walking enemy).
 3rd - Adding UI system and menus to control the game.

## Teaser Trailer

<iframe width="560" height="315" src="https://youtu.be/GiHrWDm7B6M" frameborder="0" allowfullscreen></iframe>

### Game Controls
**A** - Move player to the left.

**D** - Move player to the right.

**ESPACE** - jump/double jump (on air).

**MOUSE LEFT** - interact with UI elements that allows to click.

#### Debug Keys
**F1** - Start from the very first level.

**F2** - Start from the beginning of the current level.

**F3** - Activate/Deactivate path drawing.

**F5** - Save the current state.

**F6** - Load the previous state.

**F8** - Activate/Deactivate collider GUI drawing.

**F9** - Activate/Deactivate collider drawing.

**F10** - Activate/Deactivate god mode.

**F11** - Activate/Deactivate Framecap.

### Main Core Subsytems
(entities, UI, pathfinding, map, scene management)
This game has 5 main modules:
**Entity System** that holds all entities such as: player, enemies, collectibles and deals with their physics.
It is connected with the **pathfinding** which is in charge of calculating the path that each enemy will do and send it to him.
The **map** which reads .tmx files that hold all information of the tile based level (done in [Tiled](http://www.mapeditor.org/))(position and texture of each tile, collisions and entity position).
The **UI** with a polymorphism system of UI elements that can be created and managed by the GUI module and stored in menu structs to deal with them in groups.
And finally but not less important, the **scene management**, which is in charge of deliverying to the renderer the current level, menu and entities on screen as well as managing the transitions between them.

### Developer Team

<img src="https://drive.google.com/uc?id=1vQUdU2pbTyUeGBdSpkp46poJoaWvpupi">
[Adrià Ferrer](https://github.com/Adria-F)

- Entity system.
- Half of individual entities creation.
- Half of pathfinding algorythm.
- Design of the UI.
- Half of UI elements implementation.

<img src="https://drive.google.com/uc?id=14TNHnLRNvDjkyJJU8W_ZczK7v8pFBAsf">
[Norman Benet](https://github.com/Normanbg)

- Half of individual entities creation.
- Half of pathfinding algorythm.
- Desing of the levels and difficulty.
- Menu composition and behaviour.
- Half of UI elements implementation.

## Check Out the repository [here](https://github.com/Adria-F/Game-Development)

## Download the last version of N.A.T.P [here](https://github.com/Adria-F/Game-Development/releases/tag/2.0)


### License
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.