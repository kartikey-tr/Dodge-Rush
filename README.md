# Dodge Racer

A console-based vertical racing game in C where the player ('▲') dodges incoming traffic ('##') on a 3-lane road.

### Completed Objectives
- [x] **Difficulty Levels**: Implemented 5 distinct speed levels ranging from Baby Mode (1) to Nightmare (5).
- [x] **Multiple Obstacles**: Added an array-based grid system to handle multiple falling enemies simultaneously.
- [x] **Start Menu System**: Created an interactive menu for difficulty selection before the game begins.
- [x] **Game Restart Option**: Added functionality to restart the game immediately after a "Game Over" event.
- [x] **High Score Storage**: Implemented file handling to save and retrieve the highest score persistently.

### Controls
- **Left Arrow / 'A'**: Move Left
- **Right Arrow / 'D'**: Move Right
- **Number Keys (1-5)**: Select Difficulty
- **Enter**: Restart Game
- **Esc**: Quit Game

### How to Compile & Run
1. Compile the code:
   ```bash
   gcc game.c -o dodgeracer
