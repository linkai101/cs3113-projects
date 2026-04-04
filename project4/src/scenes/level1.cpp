#include "scenes/level1.h"
#include "utils/color.h"

Level1::Level1(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, getTilePosition(SPAWN_POSITION), assets)
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

  entities.push_back(std::make_unique<Entity>(getTilePosition(GOAL_SPAWN_POSITION), mapAnimator));
  levelGoal = entities.back().get();
  levelGoal->playAnimation("map-4");
  levelGoal->enablePhysics(Vector2{32, 32}, Vector2{-16, -16}, true);
  backgroundEntities.push_back(levelGoal);

  // Crabby
  entities.push_back(std::make_unique<Crabby>(
    getTilePosition(Vector2{4.5f, 4.5f}), // position
    assets
  ));
  crabby = static_cast<Crabby*>(entities.back().get());
  backgroundEntities.push_back(crabby);
}

void Level1::resolveCollisions(Player* player) {
  if (!loaded) return;

  // Check player collisions with level goal
  if (Entity::isColliding(player, levelGoal)) {
    requestTransition();
  }

  // Check player-crabby interactions
  bool playerSquishesCrabby = false;
  bool crabbyHitPlayer = false;
  if (Entity::isColliding(player, crabby) && !crabby->isDead()) {
    if (!player->isStunned() && !player->getPhysicsBody()->isGrounded && player->getPhysicsBody()->velocity.y > 0) {
      playerSquishesCrabby = true;
      crabby->kill();
    } else {
      crabby->reverseDirection();
      if (!player->isStunned()) crabbyHitPlayer = true;
    }
  }

  // Resolve crabby collisions
  crabby->resolveCollisions(terrainEntities);

  // Resolve player collisions
  std::vector<Entity*> playerCollidables = terrainEntities;
  playerCollidables.push_back(crabby);
  player->resolveCollisions(playerCollidables);

  // Handle player outcomes
  if (playerSquishesCrabby) {
    player->getPhysicsBody()->velocity.y = -500.0f; // Bounce player up
  } else if (crabbyHitPlayer) {
    player->hit(crabby->getPosition().x);
  }
}