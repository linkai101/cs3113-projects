#include "scenes/level1.h"
#include "utils/color.h"

Level1::Level1(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, getTilePosition(MAP_SPAWN_POSITION), assets)
{}

void Level1::loadLevel() {
  // Background image
  entities.push_back(std::make_unique<Entity>(
    Vector2{0, 0},
    Sprite(
      assets.islandBgTexture,
      Rectangle{0, 0, static_cast<float>(assets.islandBgTexture.width), static_cast<float>(assets.islandBgTexture.height)},
      Vector2{MAP_COLS * TILE_SIZE, MAP_ROWS * TILE_SIZE},
      Vector2{0, 0}
    )
  ));
  backgroundEntities.push_back(entities.back().get());
  
  // Terrain map
  loadTileGrid(&TERRAIN_MAP[0][0], MAP_ROWS, MAP_COLS, assets.islandTerrainSheet, {0, 0}, true, false);

  // Level goal
  Animator mapAnimator = Animator(
    &assets.smallMapSheet,
    Vector2{64, 64}, // size
    Vector2{32, 32} // origin
  );
  mapAnimator.addAnimation("map-1", Animator::Animation{"map-1", {0, 1, 2, 3, 4, 5, 6, 7}, 10, true});
  mapAnimator.addAnimation("map-2", Animator::Animation{"map-2", {8, 9, 10, 11, 12, 13, 14, 15}, 10, true});
  mapAnimator.addAnimation("map-3", Animator::Animation{"map-3", {16, 17, 18, 19, 20, 21, 22, 23}, 10, true});
  mapAnimator.addAnimation("map-4", Animator::Animation{"map-4", {24, 25, 26, 27, 28, 29, 30, 31}, 10, true});

  entities.push_back(std::make_unique<Entity>(
    getTilePosition(Vector2{7.5f, 3.5f}), // position
    mapAnimator
  ));
  levelGoal = entities.back().get();
  levelGoal->playAnimation("map-4");
  backgroundEntities.push_back(levelGoal);
}

void Level1::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;

  // Check collisions with level goal
  if (Entity::isColliding(player, levelGoal)) {
    requestTransition();
  }

  Scene::resolveCollisions(player);
}