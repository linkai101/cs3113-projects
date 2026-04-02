#include "scene.h"

Scene::Scene(
  int screenWidth, int screenHeight, Vector2 spawnPosition,
  Spritesheet& tilesSheet
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  spawnPosition(spawnPosition),
  tilesSheet(tilesSheet)
{}

void Scene::load(Player* player) {
  if (loaded) return;

  loadMap();
  resetPlayer(player);

  loaded = true;
}

void Scene::unload() {
  if (!loaded) return;

  staticEnvironmentEntities.clear();
  collidableEnvironmentEntities.clear();

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

  for (auto& entity : collidableEnvironmentEntities) {
    entity->update(deltaTime);
  }
  for (auto& entity : staticEnvironmentEntities) {
    entity->update(deltaTime);
  }

  player->update(deltaTime);
  resolveCollisions(player);
}

void Scene::render(Player* player) const {
  if (!loaded) return;
  
  for (auto& entity : staticEnvironmentEntities) {
    entity->render();
  }

  for (auto& entity : collidableEnvironmentEntities) {
    entity->render();
  }

  player->render();
}


Vector2 Scene::getPositionFromTileCoordinates(Vector2 tileCoordinates, int width, int height) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(height - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
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
  for (auto& entity : collidableEnvironmentEntities) {
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
  for (auto& entity : collidableEnvironmentEntities) {
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

void Scene::createBackgroundLayer(Texture2D texture) {
  float aspect = static_cast<float>(texture.width) / static_cast<float>(texture.height);
  float destHeight = static_cast<float>(screenHeight);
  float destWidth = destHeight * aspect;
  float x = (static_cast<float>(screenWidth) - destWidth) / 2.0f;

  staticEnvironmentEntities.push_back(std::make_unique<Entity>(
    Vector2{ x, 0 }, // top-left of layer
    Sprite(
      texture,
      Rectangle{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
      Vector2{ destWidth, destHeight },
      Vector2{ 0, 0 }
    )
  ));
}

void Scene::createTile(Vector2 tileCoordinates, int frameIndex, bool enablePhysics) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  Entity tile = Entity(
    position,
    Sprite(
      &tilesSheet,
      frameIndex,
      Vector2{ TILE_SIZE, TILE_SIZE }, // size
      Vector2{ 0, 0 } // origin
    )
  );
  if (enablePhysics) {
    tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);
  }

  collidableEnvironmentEntities.push_back(std::make_unique<Entity>(tile));
}