#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include "scenes/world.h"
#include "entities/enemy/zombie.h"
#include "entities/enemy/giant.h"
#include "entities/enemy/ghoul.h"
#include "sound_manager.h"
#include "utils/collision.h"

World::World(int screenWidth, int screenHeight, Assets& assets, std::string levelPath) :
  Scene(screenWidth, screenHeight, assets),
  camera(screenWidth, screenHeight),
  hotbar(screenWidth, screenHeight, assets),
  levelPath(std::move(levelPath))
{}

void World::load() {
  if (loaded) return;

  loadLevel(levelPath);

  // Add boundary walls
  float mapW = mapCols * TILE_SIZE;
  float mapH = mapRows * TILE_SIZE;
  float margin = -TILE_SIZE * 0.25f;
  float thick = 2000.0f;
  collisionBoxes.push_back({ margin - thick, margin - thick, thick, mapH - margin * 2 + thick * 2 });
  collisionBoxes.push_back({ mapW - margin, margin - thick, thick, mapH - margin * 2 + thick * 2 });
  collisionBoxes.push_back({ margin - thick, margin - thick, mapW - margin * 2 + thick * 2, thick });
  collisionBoxes.push_back({ margin - thick, mapH - margin, mapW - margin * 2 + thick * 2, thick });

  entities.push_back(std::make_unique<Player>(getTilePosition(SPAWN_POSITION), assets));
  player = dynamic_cast<Player*>(entities.back().get());

  entities.push_back(std::make_unique<Dummy>(getTilePosition({12.5f, 4.5f}), assets));
  dummy = dynamic_cast<Dummy*>(entities.back().get());

  // Spawn zombies
  for (Vector2 pos : std::initializer_list<Vector2>{
    {6.5f, 9.5f},
    {10.5f, 9.5f},
    {14.5f, 9.5f},
    {18.5f, 9.5f},
  }) {
    entities.push_back(std::make_unique<Zombie>(getTilePosition(pos), assets));
    Enemy* e = dynamic_cast<Enemy*>(entities.back().get());
    e->setTarget(player);
    enemies.push_back(e);
  }

  // Spawn giants
  for (Vector2 pos : std::initializer_list<Vector2>{
    {22.5f, 9.5f},
  }) {
    entities.push_back(std::make_unique<Giant>(getTilePosition(pos), assets));
    Enemy* e = dynamic_cast<Enemy*>(entities.back().get());
    e->setTarget(player);
    enemies.push_back(e);
  }

  // Spawn ghouls
  for (Vector2 pos : std::initializer_list<Vector2>{
    {5.5f, 15.5f},
  }) {
    entities.push_back(std::make_unique<Ghoul>(getTilePosition(pos), assets));
    Ghoul* g = dynamic_cast<Ghoul*>(entities.back().get());
    g->setTarget(player);
    enemies.push_back(g);
    ghouls.push_back(g);
  }

  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({4.5f, 7.0f}), Gun::Type::RIFLE, 30, assets));
  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({6.0f, 7.0f}), Gun::Type::PISTOL, 12, assets));
  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({7.5f, 7.0f}), Gun::Type::SHOTGUN, 6, assets));

  bandages.push_back(std::make_unique<Bandage>(getTilePosition({9.0f, 7.0f}), assets));

  camera.init(player->getPosition());
  camera.setBounds(mapCols * TILE_SIZE, mapRows * TILE_SIZE);

  Scene::load();

  SoundManager::get().playMusic(MusicTrack::AMBIENT_WIND);
}

void World::unload() {
  if (!loaded) return;

  terrain.clear();
  collisionBoxes.clear();
  entities.clear();
  bullets.clear();
  ghoulAxes.clear();
  enemies.clear();
  ghouls.clear();
  ammoCrates.clear();
  bandages.clear();

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

  // Player equip
  if (IsKeyPressed(KEY_Q)) player->equip(Player::EquipSlot::NONE);
  if (IsKeyPressed(KEY_ONE)) player->equip(Player::EquipSlot::BAT);
  if (IsKeyPressed(KEY_TWO)) player->equip(Player::EquipSlot::RIFLE);
  if (IsKeyPressed(KEY_THREE)) player->equip(Player::EquipSlot::PISTOL);
  if (IsKeyPressed(KEY_FOUR)) player->equip(Player::EquipSlot::SHOTGUN);

  // Player attack
  bool attack = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE);
  if (attack && player->canAttack()) {
    player->attack();

    // Spawn bullets if attacking with a gun
    if (auto gun = dynamic_cast<Gun*>(player->getEquipped())) {
      Gun::Type type = gun->getType();
      Gun::Properties properties = gun->getProperties();

      switch (type) {
        case Gun::Type::PISTOL:  SoundManager::get().play(SFX::PISTOL_FIRE);  break;
        case Gun::Type::RIFLE:   SoundManager::get().play(SFX::RIFLE_FIRE);   break;
        case Gun::Type::SHOTGUN: SoundManager::get().play(SFX::SHOTGUN_FIRE); break;
      }

      spawnBullets(type, properties, player->getAimAngle(), static_cast<int>(properties.bulletCount), properties.bulletSpread);
    }
  } else if (attack) {
    if (auto gun = dynamic_cast<Gun*>(player->getEquipped())) {
      if (gun->getCurrentMag() == 0 && !gun->isReloading()) {
        SoundManager::get().play(SFX::GUN_EMPTY);
      }
    }
  }

  // Player reload
  if (IsKeyPressed(KEY_R)) player->reload();
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

  // Update all bullets
  for (auto& bullet : bullets) {
    bullet->update(deltaTime);
  }

  // Spawn axes from ghouls
  for (Ghoul* ghoul : ghouls) {
    if (ghoul->hasPendingAxe()) {
      auto [origin, angle] = ghoul->consumePendingAxe();
      ghoulAxes.push_back(std::make_unique<GhoulAxe>(origin, angle, assets));
    }
  }

  // Update ghoul axes
  for (auto& axe : ghoulAxes) {
    axe->update(deltaTime);
  }

  // Resolve player collisions other entities and static colliders
  if (player) {
    std::vector<Entity*> collidables;
    for (auto& e : entities) {
      if (e.get() != player && e->getCollider().has_value()) { // non-player physics-enabled entities
        collidables.push_back(e.get());
      }
    }
    player->resolveCollisions(collidables);
    player->resolveCollisions(collisionBoxes);
  }

  // Resolve enemy collisions against collision boxes
  if (!collisionBoxes.empty()) {
    for (Enemy* enemy : enemies) {
      enemy->resolveCollisions(collisionBoxes);
    }
  }

  // Remove expired bullets
  bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
      [](const std::unique_ptr<Bullet>& b) { return b->isExpired(); }),
    bullets.end()
  );

  // Remove expired axes
  ghoulAxes.erase(
    std::remove_if(ghoulAxes.begin(), ghoulAxes.end(),
      [](const std::unique_ptr<GhoulAxe>& a) { return a->isExpired(); }),
    ghoulAxes.end()
  );

  // Axe-collision box check
  ghoulAxes.erase(
    std::remove_if(ghoulAxes.begin(), ghoulAxes.end(),
      [this](const std::unique_ptr<GhoulAxe>& a) {
        for (const Rectangle& box : collisionBoxes) {
          if (CheckPointInRect(a->getPosition(), box)) return true;
        }
        return false;
      }),
    ghoulAxes.end()
  );

  // Axe-player collision
  if (player) {
    std::optional<Rectangle> playerCollider = player->getCollider();
    if (playerCollider) {
      ghoulAxes.erase(
        std::remove_if(ghoulAxes.begin(), ghoulAxes.end(),
          [this, &playerCollider](const std::unique_ptr<GhoulAxe>& a) {
            if (CheckPointInRect(a->getPosition(), *playerCollider)) {
              player->takeDamage(a->getDamage());
              return true;
            }
            return false;
          }),
        ghoulAxes.end()
      );
    }
  }

  // Bullet-collision box check
  bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
      [this](const std::unique_ptr<Bullet>& b) {
        for (const Rectangle& box : collisionBoxes) {
          if (CheckPointInRect(b->getPosition(), box)) return true;
        }
        return false;
      }),
    bullets.end()
  );

  // Bullet-dummy collision
  if (dummy) {
    std::optional<Rectangle> dummyCollider = dummy->getCollider();
    if (dummyCollider) {
      bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
          [this, dummyCollider](const std::unique_ptr<Bullet>& b) {
            if (CheckPointInRect(b->getPosition(), *dummyCollider)) {
              dummy->takeDamage(b->getDamage());
              return true;
            }
            return false;
          }),
        bullets.end()
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

  // Bullet-enemy collision
  for (Enemy* enemy : enemies) {
    std::optional<Rectangle> enemyCollider = enemy->getCollider();
    if (enemyCollider) {
      bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
          [enemy, enemyCollider](const std::unique_ptr<Bullet>& b) {
            if (CheckPointInRect(b->getPosition(), *enemyCollider)) {
              enemy->takeDamage(b->getDamage());
              return true;
            }
            return false;
          }),
        bullets.end()
      );
    }
  }

  // Melee-enemy collision
  if (player) {
    Melee* activeMelee = dynamic_cast<Melee*>(player->getEquipped());
    std::optional<Rectangle> meleeHit = activeMelee ? activeMelee->getHitRect() : std::nullopt;
    if (!meleeHit) {
      playerMeleeHitRegistered = false;
    } else if (!playerMeleeHitRegistered) {
      for (Enemy* enemy : enemies) {
        std::optional<Rectangle> enemyCollider = enemy->getCollider();
        if (enemyCollider && CheckRectCollision(*meleeHit, *enemyCollider)) {
          enemy->takeDamage(activeMelee->getDamage());
          playerMeleeHitRegistered = true;
        }
      }
    }
  }

  // Ammo pickup
  if (player) {
    std::optional<Rectangle> playerCollider = player->getCollider();
    if (playerCollider) {
      ammoCrates.erase(
        std::remove_if(ammoCrates.begin(), ammoCrates.end(),
          [this, &playerCollider](const std::unique_ptr<AmmoCrate>& pickup) {
            std::optional<Rectangle> pickupCollider = pickup->getCollider();
            if (!pickupCollider) return false;
            if (CheckRectCollision(*playerCollider, *pickupCollider)) {
              player->addAmmo(pickup->getGunType(), pickup->getAmount());
              SoundManager::get().play(SFX::PICKUP);
              return true;
            }
            return false;
          }),
          ammoCrates.end()
      );
    }
  }

  // Bandage pickup
  if (player) {
    std::optional<Rectangle> playerCollider = player->getCollider();
    if (playerCollider) {
      bandages.erase(
        std::remove_if(bandages.begin(), bandages.end(),
          [this, &playerCollider](const std::unique_ptr<Bandage>& bandage) {
            std::optional<Rectangle> bandageCollider = bandage->getCollider();
            if (!bandageCollider) return false;
            if (
              CheckRectCollision(*playerCollider, *bandageCollider) &&
              player->getHealth() < Player::MAX_HEALTH // health not full
            ) {
              player->heal(Bandage::HEAL_AMOUNT);
              SoundManager::get().play(SFX::BANDAGE);
              return true;
            }
            return false;
          }),
        bandages.end()
      );
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
  sorted.reserve(entities.size() + ammoCrates.size() + bandages.size());
  for (auto& entity : entities) sorted.push_back(entity.get());
  for (auto& crate : ammoCrates) {
    if (Entity* e = crate->getGroundEntity()) sorted.push_back(e);
  }
  for (auto& bandage : bandages) {
    if (Entity* e = bandage->getGroundEntity()) sorted.push_back(e);
  }
  std::sort(sorted.begin(), sorted.end(), [](const Entity* a, const Entity* b) {
    return a->getPosition().y < b->getPosition().y;
  });
  for (auto* entity : sorted) entity->render();

  // Render bullets and axes above all entities
  for (auto& bullet : bullets) bullet->render();
  for (auto& axe : ghoulAxes) axe->render();

  EndMode2D();
}

void World::renderHUD() const {
  if (player) hotbar.render(
    player->getHealth(),
    Player::MAX_HEALTH,
    player->getEquipped(),
    player->getAmmoInventory(Gun::Type::RIFLE),
    player->getAmmoInventory(Gun::Type::PISTOL),
    player->getAmmoInventory(Gun::Type::SHOTGUN)
  );

  // Render cursor
  Vector2 mouse = GetMousePosition();
  DrawTexturePro(
    assets.cursorTexture,
    {0, 0, (float)assets.cursorTexture.width, (float)assets.cursorTexture.height},
    {mouse.x, mouse.y, CURSOR_RENDER_SIZE.x, CURSOR_RENDER_SIZE.y},
    {CURSOR_RENDER_SIZE.x / 2.0f, CURSOR_RENDER_SIZE.y / 2.0f},
    0.0f,
    WHITE
  );
}

float World::getPlayerDamageFlashIntensity() const {
  return player ? player->getDamageFlashIntensity() : 0.0f;
}

void World::loadLevel(const std::string& path) {
  std::ifstream file(path);
  std::string line;
  std::string currentLayer;
  std::vector<std::vector<int>> currentGrid;
  bool inColliders = false;

  auto loadLayer = [&]() {
    if (!currentLayer.empty() && !currentGrid.empty()) {
      loadLayerFromGrid(currentLayer, currentGrid);
      currentGrid.clear();
    }
  };

  while (std::getline(file, line)) {
    if (line.empty()) continue;
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "cols" || token == "rows") {
      // Ignore, info only
    } else if (token == "layer") {
      loadLayer();
      inColliders = false;
      iss >> currentLayer;
    } else if (token == "colliders") {
      loadLayer();
      inColliders = true;
    } else if (inColliders) {
      float col = std::stof(token);
      float row, w, h;
      iss >> row >> w >> h;
      collisionBoxes.push_back({ col * TILE_SIZE, row * TILE_SIZE, w * TILE_SIZE, h * TILE_SIZE });
    } else {
      // Layer grid row
      std::vector<int> gridRow;
      gridRow.push_back(std::stoi(token));
      int val;
      while (iss >> val) gridRow.push_back(val);
      currentGrid.push_back(std::move(gridRow));
    }
  }
  loadLayer();
}

void World::loadLayerFromGrid(const std::string& layerName, const std::vector<std::vector<int>>& grid) {
  Spritesheet* sheet = assets.getTileSheet(layerName);
  if (!sheet) return;

  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  mapRows = rows;
  mapCols = cols;

  std::vector<int> flat;
  flat.reserve(rows * cols);
  for (const auto& row : grid) flat.insert(flat.end(), row.begin(), row.end());

  loadTileGrid(terrain, flat.data(), rows, cols, *sheet, {0, 0}, false);
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
    bullets.push_back(std::make_unique<Bullet>(origin, angle, type, properties, assets));
  };

  for (int i = 0; i < bulletCount; ++i) {
    float offset = (i - (bulletCount - 1) / 2.0f) * spread;
    spawn(spawnAngle + offset);
  }
}