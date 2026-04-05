# High Tide Heist
For CS-UY 3113's Project 4. A beach-side treasure hunt platformer.

## Class organization

The entry point of the game is in `src/main.cpp`.

The game is organized into the following classes:
- **`src/game.h`**: The main game class, responsible for running the game loop.
  - Stores assets, scenes, and player. Controls the active scene and handles game reset.
- **`src/scenes/scene.h`**: Base class for all scenes (levels, menus, screens).
  - Stores entities and terrain. Handles tile grid loading, entity rendering, and collision resolution. Subclassed by `level1.h`, `level2.h`, `level3.h`, `main_menu.h`, `win_screen.h`, and `lose_screen.h`.
- **`src/entities/entity.h`**: Base class for all game entities.
  - Stores position, physics body, and the sprite/animator for rendering. Handles individual entity updates and collision resolution.
- **`src/entities/player.h`**: The player entity (the Captain).
  - Handles input, jumping, lives, hit/stun logic.
- **`src/entities/crabby.h`**: An enemy that wanders back and forth and harms the player on contact.
- **`src/entities/tooth.h`**: An enemy that wanders, chases the player when nearby, and performs melee attacks.
- **`src/entities/star.h`**: An enemy that idles, chases the player when nearby, then rolls into an attack.
- **`src/assets.h`**: Struct that loads and stores all textures, spritesheets, music, and sounds.
- `src/components/`
  - **`sprite.h`**, **`spritesheet.h`**, **`animator.h`**: Handles texture rendering and calls DrawTexturePro.
  - **`physics_body.h`**: Stores physics state and updates velocity.
  - **`follow_camera.h`**: A lerp-smoothed 2D camera that tracks a target position.

## Assets

### Textures
Some texture assets were reformatted into custom spritesheets for use in the game.
- [Treasure Hunters by Pixel Frog](https://pixelfrog-assets.itch.io/treasure-hunters)
  - `captain.png` (64x40 sprites, 6 columns)
    - idle (0-4), run (6-11), jump (12-14), fall (18), ground (24-25), hit (30-33), dead-hit (36-39), dead-ground (42-45)
  - `crabby.png` (72x32 sprites, 9 columns)
    - idle (0-8), run (9-14), jump (18-20), fall (27), ground (36-37), anticipation (45-47), attack (54-57), hit (63-66), dead-hit (72-75), dead-ground (81-84), attack effect excluded
  - `island-bg.png`
  - `island-terrain.png` (32x32 sprites, 17 columns)
  - `pirate-ship-platforms.png` (32x32 sprites, 4 columns)
  - `pirate-ship-terrain.png` (32x32 sprites, 17 columns)
  - `small-map.png` (20x20 sprites, 8 columns)
  - `star.png` (34x30, 8 columns)
    - idle (0-7), run (8-13), jump (16-18), fall (24), ground (32-33), anticipation (40-42), attack (48-51), hit (56-59), dead-hit (64-67), dead-ground (72-75), attack effect excluded
  - `tooth.png` (34x30 sprites, 8 columns)
    - idle (0-7), run (8-13), jump (16-18), fall (24), ground (32-33), anticipation (40-42), attack (48-52), hit (56-59), dead-hit (64-67), dead-ground (72-75), attack effect excluded

- [Beach & Camping Ambient Sound Pack](https://audialucas.itch.io/beach-camping-ambient-sound-pack)
  - `waves-loop.wav`

- [Brackeys' Platformer Bundle](https://brackeysgames.itch.io/brackeys-platformer-bundle)
  - `jump.wav`
  - `hurt.wav`
  - `tap.wav`