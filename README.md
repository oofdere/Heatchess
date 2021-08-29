# Heatchess
The hottest chess client around.

## Current state of the codebase
This code is only a simple proof of concept to demonstrate the idea. It is a somewhat useless codebase, as I hadn't thought about the architecture of this program beforehand and got straight into coding. So this is just a simple demo in its current state. Pieces cannot move. **It touches memory it shouldn't.** You can load a board by putting a FEN into the clipboard and pressing  `v` in the game window.
## Goals
 - [ ] Modular architecture with first-class plugin support.
 - [ ] Launcher with auto-update and plugin manager.
 - [ ] Works on Linux, Windows, Android, and in the web browser.
 - [ ] Arbitrary board and piece definitions.
 - [ ] Suckless, while not being intimidating for normies.
 - [ ] 2D and 3D rendering support.
 - [ ] UCI engine support.
 - [ ] P2P multiplayer support over WebRTC/WebSockets/similar.
## Architecture
 - board.h
 - board.c
 - pieces/
   - pawn.h
   - rook.h
   - knight.h
   - bishop.h
   - queen.h
   - king.h
 - logic/
   - piece.h
   - piece.c
 - graphics/
   - menu.h
   - menu.c
 - multiplayer/
 - plugins/
 - launcher/
   - update.c
   - mods.c
   - 
 - main.c
 - Makefile

## License
TODO: Choose a license