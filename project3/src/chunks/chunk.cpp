#include "chunk.h"

Chunk::Chunk(
  int screenWidth, int screenHeight,
  Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  tilesSheet(tilesSheet),
  grassSheet(grassSheet),
  waterSheet(waterSheet)
{}

void Chunk::load() {
  if (loaded) return;

  loadMap();

  loaded = true;
}

void Chunk::unload() {
  if (!loaded) return;

  staticEntities.clear();
  collidableEntities.clear();

  loaded = false;
}

void Chunk::update(float deltaTime) {
  if (!loaded) return;

  for (auto& entity : collidableEntities) {
    entity->update(deltaTime);
  }

  for (auto& entity : staticEntities) {
    entity->update(deltaTime);
  }
}

void Chunk::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Check collisions with collidable entities
  pb.isGrounded = false;
  // Resolve horizontal collisions
  for (auto& entity : collidableEntities) {
    if (!entity->getPhysicsBody()) continue;

    Rectangle playerBox = pb.getCollider(player->getPosition());
    Rectangle entityBox = entity->getPhysicsBody()->getCollider(entity->getPosition());

    if (!CheckCollisionRecs(playerBox, entityBox)) continue;

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
  for (auto& entity : collidableEntities) {
    if (!entity->getPhysicsBody()) continue;

    Rectangle playerBox = pb.getCollider(player->getPosition());
    Rectangle entityBox = entity->getPhysicsBody()->getCollider(entity->getPosition());

    if (!CheckCollisionRecs(playerBox, entityBox)) continue;

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

void Chunk::render() const {
  if (!loaded) return;
  
  for (auto& entity : staticEntities) {
    entity->render();
  }
  for (auto& entity : collidableEntities) {
    entity->render();
  }
}


void Chunk::setExit(ExitDirection direction, std::optional<Chunk*> nextChunk) {
  switch (direction) {
    case EXIT_LEFT:
      leftExit = nextChunk;
      break;
    case EXIT_RIGHT:
      rightExit = nextChunk;
      break;
    case EXIT_TOP:
      topExit = nextChunk;
      break;
    case EXIT_BOTTOM:
      bottomExit = nextChunk;
      break;
  }
}

std::optional<Chunk*> Chunk::handleExit(Player* player) {
  if (!player->getPhysicsBody()) return std::nullopt;
  PhysicsBody& pb = *player->getPhysicsBody();
  Rectangle playerBox = pb.getCollider(player->getPosition());

  // Transition triggers when the entire bounding box is out of frame.
  // On entry, position is set so the entire bounding box is just inside the new chunk.
  if (leftExit.has_value() && playerBox.x + playerBox.width < 0) {
    player->getPosition().x = static_cast<float>(screenWidth) - pb.colliderSize.x - pb.colliderOffset.x;
    return leftExit.value();
  }
  if (rightExit.has_value() && playerBox.x > static_cast<float>(screenWidth)) {
    player->getPosition().x = -pb.colliderOffset.x;
    return rightExit.value();
  }
  if (topExit.has_value() && playerBox.y + playerBox.height < 0) {
    player->getPosition().y = static_cast<float>(screenHeight) - pb.colliderSize.y - pb.colliderOffset.y;
    return topExit.value();
  }
  if (bottomExit.has_value() && playerBox.y > static_cast<float>(screenHeight)) {
    player->getPosition().y = -pb.colliderOffset.y;
    return bottomExit.value();
  }

  return std::nullopt;
}


Vector2 Chunk::getPositionFromTileCoordinates(Vector2 tileCoordinates, int width, int height) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(height - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
}

void Chunk::createBackgroundLayer(Texture2D texture) {
  float aspect = static_cast<float>(texture.width) / static_cast<float>(texture.height);
  float destHeight = static_cast<float>(screenHeight);
  float destWidth = destHeight * aspect;
  float x = (static_cast<float>(screenWidth) - destWidth) / 2.0f;

  staticEntities.push_back(std::make_unique<Entity>(
    Vector2{ x, 0 }, // top-left of layer
    Sprite(
      texture,
      Rectangle{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
      Vector2{ destWidth, destHeight },
      Vector2{ 0, 0 }
    )
  ));
}

void Chunk::createTile(Vector2 tileCoordinates, int frameIndex, bool enablePhysics) {
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

  collidableEntities.push_back(std::make_unique<Entity>(tile));
}

void Chunk::createGrass(Vector2 tileCoordinates, int frameIndex, bool hanging) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);
  if (!hanging) position.y -= TILE_SIZE / 16 * (20 - 16);

  staticEntities.push_back(std::make_unique<Entity>(
    position,
    Sprite(
      &grassSheet,
      frameIndex,
      Vector2{ TILE_SIZE, TILE_SIZE / 16 * 20 }, // size
      Vector2{ 0, 0 } // origin
    )
  ));
}

void Chunk::createWater(Vector2 tileCoordinates, bool surface) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  if (surface) {
    Animator animator = Animator(
      &waterSheet,
      Vector2{ TILE_SIZE, TILE_SIZE }, // size
      Vector2{ 0, 0 } // origin
    );
    animator.addAnimation("surface", Animator::Animation{"surface", { 1, 2, 3, 4, 5 }, 10, true});
    animator.play("surface");

    staticEntities.push_back(std::make_unique<Entity>(
      position,
      animator
    ));
  } else {
    staticEntities.push_back(std::make_unique<Entity>(
      position,
      Sprite(
        &waterSheet,
        0,
        Vector2{ TILE_SIZE, TILE_SIZE }, // size
        Vector2{ 0, 0 } // origin
      )
    ));
  }
}