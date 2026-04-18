#include <algorithm>
#include <cmath>
#include <string>
#include "scenes/world.h"
#include "utils/collision.h"

World::World(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, assets),
  camera(screenWidth, screenHeight)
{}

void World::load() {
  if (loaded) return;

  loadTileGrid(
    terrain,
    &TERRAIN_MAP[0][0], MAP_ROWS, MAP_COLS,
    assets.terrainSheet, Vector2{0, 0},
    false
  );
  
  entities.push_back(std::make_unique<Player>(getTilePosition(SPAWN_POSITION), assets));
  player = dynamic_cast<Player*>(entities.back().get());

  entities.push_back(std::make_unique<Dummy>(getTilePosition({3.5f, 2.5f}), assets));
  dummy = dynamic_cast<Dummy*>(entities.back().get());

  for (Vector2 pos : std::initializer_list<Vector2>{
    {-6.5f, -6.5f},
    {-6.5f, 6.5f},
    {6.5f, -6.5f},
    {6.5f, 6.5f},
  }) {
    entities.push_back(std::make_unique<Zombie>(getTilePosition(pos), assets));
    Zombie* z = dynamic_cast<Zombie*>(entities.back().get());
    z->setTarget(player);
    zombies.push_back(z);
  }

  camera.init(player->getPosition());

  Scene::load();
}

void World::unload() {
  if (!loaded) return;

  entities.clear();
  zombies.clear();

  Scene::unload();
}

void World::processInput() {
  if (!loaded) return;

  // Player movement
  bool up = IsKeyDown(KEY_W);
  bool down = IsKeyDown(KEY_S);
  bool left = IsKeyDown(KEY_A);
  bool right = IsKeyDown(KEY_D);
  player->move(up, down, left, right);

  // Player attack
  bool attack = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE);
  if (attack && player->canAttack()) {
    player->attack();

    // Spawn bullets if attacking with a gun
    if (auto gun = dynamic_cast<Gun*>(player->getEquipped())) {
      Gun::Type type = gun->getType();
      Gun::Properties properties = gun->getProperties();

      spawnBullets(type, properties, player->getAimAngle(), static_cast<int>(properties.bulletCount), properties.bulletSpread);
    }
  }

  // Player reload
  if (IsKeyPressed(KEY_R)) player->reload();

  // DEBUG
  if (IsKeyPressed(KEY_ONE)) player->debug(1); // equip hands
  if (IsKeyPressed(KEY_TWO)) player->debug(2); // equip bat
  if (IsKeyPressed(KEY_THREE)) player->debug(3); // equip rifle
  if (IsKeyPressed(KEY_FOUR)) player->debug(4); // equip pistol
  if (IsKeyPressed(KEY_FIVE)) player->debug(5); // equip shotgun

  if (IsKeyPressed(KEY_ZERO)) player->debug(0); // die
}

void World::update(float deltaTime) {
  if (!loaded) return;

  // Pass mouse position to player
  if (player) {
    player->setMouseWorldPosition(GetScreenToWorld2D(GetMousePosition(), camera.get()));
  }

  // Update all entities
  for (auto& entity : entities) {
    entity->update(deltaTime);
  }

  // Resolve player collisions against all physics-enabled non-player entities
  if (player) {
    std::vector<Entity*> collidables;
    for (auto& e : entities) {
      if (e.get() != player && e->getCollider().has_value()) {
        collidables.push_back(e.get());
      }
    }
    player->resolveCollisions(collidables);
  }

  // Remove expired bullets
  entities.erase(
    std::remove_if(entities.begin(), entities.end(),
      [](const std::unique_ptr<Entity>& e) {
        auto* b = dynamic_cast<Bullet*>(e.get());
        return b && b->isExpired();
      }),
    entities.end()
  );

  // Bullet-dummy collision
  if (dummy) {
    std::optional<Rectangle> dummyCollider = dummy->getCollider();
    if (dummyCollider) {
      entities.erase(
        std::remove_if(entities.begin(), entities.end(),
          [this, dummyCollider](const std::unique_ptr<Entity>& e) {
            Bullet* b = dynamic_cast<Bullet*>(e.get());
            if (!b) return false;
            if (CheckPointInRect(b->getPosition(), *dummyCollider)) {
              dummy->takeDamage(b->getDamage());
              return true;
            }
            return false;
          }),
        entities.end()
      );
    }
  }

  // Melee-dummy collision
  if (player && dummy) {
    Melee* activeMelee = dynamic_cast<Melee*>(player->getEquipped());
    std::optional<Rectangle> meleeHit = activeMelee ? activeMelee->getHitRect() : std::nullopt;
    if (!meleeHit) {
      playerMeleeHitRegistered = false;
    } else if (!playerMeleeHitRegistered) {
      std::optional<Rectangle> dummyCollider = dummy->getCollider();
      if (dummyCollider && CheckRectCollision(*meleeHit, *dummyCollider)) {
        dummy->takeDamage(activeMelee->getDamage());
        playerMeleeHitRegistered = true;
      }
    }
  }

  // Bullet-zombie collision
  for (Zombie* zombie : zombies) {
    std::optional<Rectangle> zombieCollider = zombie->getCollider();
    if (zombieCollider) {
      entities.erase(
        std::remove_if(entities.begin(), entities.end(),
          [zombie, zombieCollider](const std::unique_ptr<Entity>& e) {
            Bullet* b = dynamic_cast<Bullet*>(e.get());
            if (!b) return false;
            if (CheckPointInRect(b->getPosition(), *zombieCollider)) {
              zombie->takeDamage(b->getDamage());
              return true;
            }
            return false;
          }),
        entities.end()
      );
    }
  }

  // Melee-zombie collision
  if (player) {
    Melee* activeMelee = dynamic_cast<Melee*>(player->getEquipped());
    std::optional<Rectangle> meleeHit = activeMelee ? activeMelee->getHitRect() : std::nullopt;
    if (!meleeHit) {
      playerMeleeHitRegistered = false;
    } else if (!playerMeleeHitRegistered) {
      for (Zombie* zombie : zombies) {
        std::optional<Rectangle> zombieCollider = zombie->getCollider();
        if (zombieCollider && CheckRectCollision(*meleeHit, *zombieCollider)) {
          zombie->takeDamage(activeMelee->getDamage());
          playerMeleeHitRegistered = true;
          break;
        }
      }
    }
  }

  // Update camera
  if (player) {
    camera.update(deltaTime, player->getPosition());
  }
}

void World::render() const {
  if (!loaded) return;

  BeginMode2D(camera.get());

  // Render terrain entities
  for (auto& entity : terrain) {
    entity->render();
  }

  // Render entities by y position
  std::vector<Entity*> sorted;
  sorted.reserve(entities.size());
  for (auto& entity : entities) sorted.push_back(entity.get());
  std::sort(sorted.begin(), sorted.end(), [](const Entity* a, const Entity* b) {
    return a->getPosition().y < b->getPosition().y;
  });
  for (auto* entity : sorted) entity->render();

  EndMode2D();

  // Health HUD
  if (player) {
    float health = player->getHealth();
    float maxHealth = Player::MAX_HEALTH;
    float ratio = health / maxHealth;

    const int barWidth = 200;
    const int barHeight = 16;
    const int barX = 20;
    const int barY = screenHeight - 130;

    DrawRectangle(barX, barY, barWidth, barHeight, Color{80, 0, 0, 255});
    DrawRectangle(barX, barY, static_cast<int>(barWidth * ratio), barHeight, Color{220, 50, 50, 255});
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
    DrawText("HP", barX, barY - 22, 20, WHITE);
  }

  // Ammo HUD
  if (player) {
    const Gun* gun = dynamic_cast<const Gun*>(player->getEquipped());
    if (gun) {
      const char* weaponName = "";
      switch (gun->getType()) {
        case Gun::Type::RIFLE: weaponName = "RIFLE"; break;
        case Gun::Type::PISTOL: weaponName = "PISTOL"; break;
        case Gun::Type::SHOTGUN: weaponName = "SHOTGUN"; break;
      }
      std::string ammoText = std::to_string(gun->getCurrentMag()) + " / " + std::to_string(player->getAmmoInventory(gun->getType()));
      DrawText(weaponName, 20, screenHeight - 80, 28, WHITE);
      DrawText(ammoText.c_str(), 20, screenHeight - 44, 36, WHITE);
    }
  }
}

void World::loadTileGrid(
  std::vector<std::unique_ptr<Entity>>& vectorToAddTo,
  const int* grid, int rows, int cols,
  Spritesheet& sheet, Vector2 tileOffset,
  bool enablePhysics
) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int frameIndex = grid[r * cols + c];
      if (frameIndex == -1) continue;

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
      if (enablePhysics) {
        tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);
      }

      // Add to scene
      vectorToAddTo.push_back(std::make_unique<Entity>(tile));
    }
  }
}

Vector2 World::getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset) {
  return {
    static_cast<float>((tileOffset.x + tileCoordinates.x) * TILE_SIZE),
    static_cast<float>((tileOffset.y + tileCoordinates.y) * TILE_SIZE)
  };
}

void World::spawnBullets(Gun::Type type, Gun::Properties properties, [[maybe_unused]] float aimAngle, int bulletCount, float spread) {
  Vector2 gunPos = { player->getPosition().x, player->getPosition().y + BULLET_SPAWN_Y_OFFSET };
  Vector2 mousePos = player->getMouseWorldPosition();
  float spawnAngle = atan2f(mousePos.y - gunPos.y, mousePos.x - gunPos.x);

  Vector2 origin = {
    gunPos.x + cosf(spawnAngle) * BULLET_SPAWN_DIST,
    gunPos.y + sinf(spawnAngle) * BULLET_SPAWN_DIST
  };

  auto spawn = [this, origin, type, properties](float angle) {
    entities.push_back(std::make_unique<Bullet>(origin, angle, type, properties, assets));
  };

  for (int i = 0; i < bulletCount; ++i) {
    float offset = (i - (bulletCount - 1) / 2.0f) * spread;
    spawn(spawnAngle + offset);
  }
}