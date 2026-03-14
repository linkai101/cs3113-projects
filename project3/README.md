# Dustrider
For CS-UY 3113's Project 3. A jetpack-powered wasteland exploration platformer game.

## Class organization

The entry point of the game is in `src/main.cpp`.

The game is organized into the following classes:
- **`src/game.h`**: The main game class, responsible for running the game loop.
  - Stores resources, chunks, and player. Controls the active chunk.
- **`src/chunks/chunk.h`**: The current "chunk" that the player is in. Can be linked together to form a continuous world.
  - Stores entities. Handles entity updates, collisions, and level creation.
- **`src/entities/entity.h`**: A base class for all game entities.
  - Stores position, physics body, and the sprite/animator for rendering. Handles individual entity updates and rendering.
- `src/components/`
  - **`sprite.h`**, **`spritesheet.h`**, **`animator.h`**: Handles texture rendering and calls DrawTexturePro.
  - **`physics_body.h`**: Stores physics state and updates velocity.

## Assets

### Textures
Some texture assets were reformatted into custom spritesheets for use in the game.
- [Jet pack man by Game Developer Studio](https://gamedeveloperstudio.itch.io/jet-pack-man)
  - `jetpackman.png` (692x599 sprites, 5 columns)
    - Standing idle (0-14), standing walking (15-29), standing running (30-44), standing jumping (45-54), standing dying (55-59), flying (60-74), flying no movement (75-84), flying dying (85-89)
- [Wasteland Tileset by Quintino Pixels](https://quintino-pixels.itch.io/wasteland-plataformer-tileset)
  - `sky.png` (512x256)
  - `mountains_1.png` (512x256)
  - `mountains_2.png` (512x256)
  - `tiles.png` (16x16 sprites, 4 columns)
    - Block (0), inner (1), inner w/ fossil (2), top alt (3), sides (4-7), corner outer (8-11), corner inner (12-15), slope (16-17), bridge (18-20)
  - `water.png` (16x16 sprites, 6 columns)
    - Fill (0), surface (1-5)
  - `grass.png` (16x20 sprites, 9 columns)
- [Fireball Effect by NYKNCK](https://nyknck.itch.io/pixelarteffectfx017)
  - `meteor.png` (32x64 sprites, 5 columns)