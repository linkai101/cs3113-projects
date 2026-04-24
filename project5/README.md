# Holdout
For CS-UY 3113's Project 5. A post-apocalyptic survival game.

## Class organization

The entry point of the game is in `src/main.cpp`.

The game is organized into the following classes:
- **`src/game.h`**: The main game class, responsible for running the game loop.
  - Stores assets, scenes, and the active scene. Handles scene switching.
- **`src/assets.h`**: Struct that loads and stores all textures, spritesheets, sounds, and music.
- **`src/sound_manager.h`**: Singleton in charge of all sound playback for SFX and music streams.
- **`src/scenes/scene.h`**: Base class for all scenes (levels, menus).
  - **`src/scenes/world.h`**: The main gameplay scene. Stores player, enemies, bullets, terrain, and items. Handles tile level loading, wave spawning, collision resolution, and game state transitions.
  - **`src/scenes/tutorial.h`**: A tutorial scene with a training dummy and no wave system.
  - **`src/scenes/main_menu.h`**: The main menu scene.
- **`src/entities/entity.h`**: Base class for all game entities. Stores position, physics body, and the sprite/animator for rendering. Handles individual entity updates and collision resolution.
  - **`src/entities/player.h`**: The player entity. Handles input, movement, weapon equipping, attacking, reloading, health, and damage flash.
  - **`src/entities/enemy/enemy.h`**: Abstract base class for all enemies.
    - **`src/entities/enemy/zombie.h`**: A melee enemy that chases the player and attacks on contact.
    - **`src/entities/enemy/ghoul.h`**: A ranged enemy that throws axes at the player.
    - **`src/entities/enemy/giant.h`**: A heavy melee enemy with two distinct attack modes.
  - **`src/entities/bullet.h`**: A gun projectile. Travels in a direction and expires at max range.
  - **`src/entities/ghoul_axe.h`**: A spinning axe projectile spawned by the Ghoul on attack.
  - **`src/entities/dummy.h`**: A training dummy used in the tutorial. Respawns after death and shows floating damage indicators.
- **`src/items/item.h`**: Base class for all droppable world items. Handles ground-placement and collider.
  - **`src/items/equippable/equippable.h`**: Abstract base class for equippable weapons.
    - **`src/items/equippable/gun.h`**: A ranged weapon (rifle, pistol, or shotgun). Handles firing cooldown, magazine, and reloading.
    - **`src/items/equippable/melee.h`**: A melee weapon (hands or bat). Handles strike timing and hit-rectangle detection.
  - **`src/items/ammo_crate.h`**: A pickable item that restores ammo for a specific gun type.
  - **`src/items/bandage.h`**: A pickable item that heals the player.
- **`src/hud/hotbar.h`**: Renders the HUD (health bar, weapon slots, reserve ammo counts).
- `src/components/`
  - **`sprite.h`**, **`spritesheet.h`**, **`animator.h`**: Handles texture rendering and calls DrawTexturePro.
  - **`physics_body.h`**: Stores physics state and updates velocity.
  - **`follow_camera.h`**: A lerp-smoothed 2D camera that tracks a target position.

## Assets

### Textures
Some texture assets were reformatted into custom spritesheets for use in the game.
- [Post-Apocalyptic Tileset by TheLazyStone](https://thelazystone.itch.io/post-apocalypse-pixel-art-asset-pack)

### Sounds
- [Sound Effects - Survival I by Darkworld Audio](https://darkworldaudio.itch.io/sound-effects-survival-i)
- [50 Free Game Sounds Pack by Place Holder Assets](https://placeholder-assets.itch.io/50-free-sounds-pack)
- [Premium Weapon Pack by Wilkin Games](https://wilkingames.itch.io/premium-weapon-pack)