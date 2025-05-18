# SDL Game with Arduino Controller

This is a 2D game developed in **C** using **SDL 1.2**. It features a menu system, text rendering, character control, background scrolling, simple puzzles (enigmes), a mini-map, and a tic-tac-toe (XO) game. The game also integrates **Arduino** for controller (manette) input.

## Features

- Developed in **C** on **Ubuntu** using SDL 1.2
- Modules:
  - Main game loop
  - Menu and UI rendering
  - Text rendering using `SDL_ttf`
  - Player character (perso) movement and animation
  - Background scrolling
  - Puzzle and enigma logic
  - Minimap rendering
  - Tic-tac-toe game (XO)
- **Arduino-based controller (manette)** integration via serial communication

## Requirements

Install the following libraries on Ubuntu:

```bash
sudo apt-get install build-essential libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
For Arduino communication:
sudo apt-get install libserialport-dev
To compile the game:
-make clean (to delete .o files)
-make (to compile the game)
-./prog(to run the game)

Arduino Integration
The game communicates with an Arduino-based controller (manette) over serial. Make sure your Arduino is connected and that the correct port (e.g. /dev/ttyUSB0) is used in your source code.

You can upload the corresponding Arduino sketch using the Arduino IDE.
