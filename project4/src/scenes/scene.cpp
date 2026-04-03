#include "scene.h"

Scene::Scene(
  int screenWidth, int screenHeight, Vector2 spawnPosition,
  Spritesheet& islandTerrainSheet
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  spawnPosition(spawnPosition),
  islandTerrainSheet(islandTerrainSheet),
  camera(screenWidth, screenHeight)
{}

void Scene::load(Player* player) {
  if (loaded) return;

  loadLevel();
  resetPlayer(player);
  camera.init(player->getPosition());

  loaded = true;
}

void Scene::unload() {
  if (!loaded) return;

  foregroundEntities.clear();
  backgroundEntities.clear();
  entities.clear();

  loaded = false;
}

void Scene::processInput(Player* player) {
  if (!loaded) return;

  bool left = IsKeyDown(KEY_A);
  bool right = IsKeyDown(KEY_D);
  bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE);

  player->move(left, right, up);
}

void Scene::update(float deltaTime, Player* player) {
  if (!loaded) return;

  for (auto& entity : entities) {
    entity->update(deltaTime);
  }

  player->update(deltaTime);
  resolveCollisions(player);

  camera.update(deltaTime, player->getPosition());
}

void Scene::render(Player* player) const {
  if (!loaded) return;

  BeginMode2D(camera.get());

  for (auto& entity : backgroundEntities) {
    entity->render();
  }

  player->render();

  for (auto& entity : foregroundEntities) {
    entity->render();
  }

  EndMode2D();
}

void Scene::resetPlayer(Player* player) {
  player->getPosition() = spawnPosition;
  player->getPhysicsBody()->velocity = {0, 0};
  player->getPhysicsBody()->acceleration = {0, 0};
  player->getPhysicsBody()->isGrounded = false;
  player->playAnimation("idle");
}

void Scene::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Check collisions with collidable entities
  pb.isGrounded = false;
  // Resolve horizontal collisions
  for (auto& entity : entities) {
    if (!entity->getPhysicsBody()) continue;

    Rectangle playerBox = pb.getCollider(player->getPosition());
    Rectangle entityBox = entity->getPhysicsBody()->getCollider(entity->getPosition());

    if (!Entity::isColliding(player, entity.get())) continue;

    float overlapX = std::min(playerBox.x + playerBox.width,  entityBox.x + entityBox.width) - std::max(playerBox.x, entityBox.x);
    float overlapY = std::min(playerBox.y + playerBox.height, entityBox.y + entityBox.height) - std::max(playerBox.y, entityBox.y);

    if (overlapX < overlapY) {
      if (playerBox.x < entityBox.x) {
        player->getPosition().x -= overlapX;
      } else {
        player->getPosition().x += overlapX;
      }
      pb.velocity.x = 0;
    }
  }
  // Resolve vertical collisions
  for (auto& entity : entities) {
    if (!entity->getPhysicsBody()) continue;

    Rectangle playerBox = pb.getCollider(player->getPosition());
    Rectangle entityBox = entity->getPhysicsBody()->getCollider(entity->getPosition());

    if (!Entity::isColliding(player, entity.get())) continue;

    float overlapX = std::min(playerBox.x + playerBox.width,  entityBox.x + entityBox.width) - std::max(playerBox.x, entityBox.x);
    float overlapY = std::min(playerBox.y + playerBox.height, entityBox.y + entityBox.height) - std::max(playerBox.y, entityBox.y);

    if (overlapY <= overlapX) {
      if (playerBox.y < entityBox.y) { // Player landed on top
        player->getPosition().y -= overlapY;
        pb.isGrounded = true;
        pb.velocity.y = 0;
      } else { // Player hit ceiling
        player->getPosition().y += overlapY;
        pb.velocity.y = 0;
      }
    }
  }
}

void Scene::loadTileGrid(
  const int* grid, int rows, int cols,
  Spritesheet& sheet, Vector2 tileOffset,
  bool enablePhysics, bool foreground
) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int frameIndex = grid[r * cols + c];
      if (frameIndex == EMPTY_TILE) continue;

      Vector2 position = getTilePosition({static_cast<float>(c), static_cast<float>(r)}, tileOffset);

      Entity tile = Entity(
        position,
        Sprite(
          &sheet,
          frameIndex,
          Vector2{ TILE_SIZE, TILE_SIZE },
          Vector2{ 0, 0 }
        )
      );
      if (enablePhysics) tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);

      // Add to scene
      entities.push_back(std::make_unique<Entity>(tile));
      if (foreground) foregroundEntities.push_back(entities.back().get());
      else backgroundEntities.push_back(entities.back().get());
    }
  }
}

Vector2 Scene::getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset) {
  return {
    static_cast<float>((tileOffset.x + tileCoordinates.x) * TILE_SIZE),
    static_cast<float>((tileOffset.y + tileCoordinates.y) * TILE_SIZE)
  };
}