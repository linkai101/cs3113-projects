#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
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

  currentWave = 0;
  gameState = GameState::WAITING;
  stateTimer = WAVE_START_DELAY;

  spawnItems();

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
  spawnQueue.clear();
  transition = SceneTransition::NONE;

  Scene::unload();
}

void World::processInput() {
  if (!loaded) return;
  
  if (gameState == GameState::WIN || gameState == GameState::GAME_OVER) {
    if (IsKeyPressed(KEY_ENTER)) transition = SceneTransition::TO_MENU;
    player->move(false, false, false, false);
    return;
  };

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

      spawnBullets(type, properties, static_cast<int>(properties.bulletCount), properties.bulletSpread);
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

  // Resolve player collisions with other entities and static colliders
  if (player) {
    std::vector<Entity*> collidables;
    for (auto& e : entities) {
      if (e.get() != player && e->getCollider().has_value()) {
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

  // Wave state machine
  if (gameState == GameState::WIN || gameState == GameState::GAME_OVER) return;

  if (player && player->isDead()) {
    gameState = GameState::GAME_OVER;
    return;
  }

  if (gameState == GameState::WAITING) {
    stateTimer -= deltaTime;
    if (stateTimer <= 0.0f) {
      startWave();
    }
  } else if (gameState == GameState::IN_WAVE) {
    // Spawn enemies gradually
    if (spawnQueuePos < static_cast<int>(spawnQueue.size())) {
      enemySpawnTimer -= deltaTime;
      if (enemySpawnTimer <= 0.0f) {
        spawnEnemy(spawnQueue[spawnQueuePos], findSpawnPosition(MIN_ENEMY_SPAWN_DIST));
        spawnQueuePos++;
        enemySpawnTimer = ENEMY_SPAWN_INTERVAL;
      }
    }

    // Check if all enemies are dead and none left to spawn
    if (spawnQueuePos >= static_cast<int>(spawnQueue.size())) {
      bool allDead = std::all_of(enemies.begin(), enemies.end(), [](Enemy* e) { return e->isDead(); });
      if (allDead) {
        // Remove dead enemy bodies from the entity list
        entities.erase(
          std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity>& e) {
              auto* enemy = dynamic_cast<Enemy*>(e.get());
              return enemy && enemy->isDead();
            }),
          entities.end()
        );
        enemies.clear();
        ghouls.clear();
        currentWave++;
        if (currentWave >= NUM_WAVES) {
          gameState = GameState::WIN;
        } else {
          spawnItems();
          stateTimer = WAVE_BETWEEN_DELAY;
          gameState = GameState::WAITING;
        }
      }
    }
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
  if (gameState == GameState::WIN || gameState == GameState::GAME_OVER) {
    DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 160 });

    const char* headline = (gameState == GameState::WIN) ? "YOU WIN!" : "GAME OVER";
    Color headlineColor = (gameState == GameState::WIN) ? YELLOW : RED;
    int headlineSize = 72;
    DrawText(headline,
      screenWidth / 2 - MeasureText(headline, headlineSize) / 2,
      screenHeight / 2 - 80,
      headlineSize, headlineColor);

    const char* prompt = "Press ENTER to return to menu";
    int promptSize = 24;
    DrawText(prompt,
      screenWidth / 2 - MeasureText(prompt, promptSize) / 2,
      screenHeight / 2 + 40,
      promptSize, WHITE);
    return;
  }

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

void World::startWave() {
  const WaveEnemyCounts& counts = WAVE_ENEMY_COUNTS[currentWave];
  spawnQueue.clear();
  spawnQueuePos = 0;

  for (int i = 0; i < counts.zombies; i++) spawnQueue.push_back(EnemyType::ZOMBIE);
  for (int i = 0; i < counts.giants; i++) spawnQueue.push_back(EnemyType::GIANT);
  for (int i = 0; i < counts.ghouls; i++) spawnQueue.push_back(EnemyType::GHOUL);

  // Shuffle spawn queue
  std::mt19937 rng{static_cast<unsigned>(GetTime() * 1000)}; // random number generator, using time as seed
  std::shuffle(spawnQueue.begin(), spawnQueue.end(), rng);

  enemySpawnTimer = 0.0f;
  gameState = GameState::IN_WAVE;
}

void World::spawnItems() {
  const WaveItemCounts& counts = WAVE_ITEM_COUNTS[currentWave];
  for (int i = 0; i < counts.rifleCrates; i++)
    ammoCrates.push_back(std::make_unique<AmmoCrate>(findSpawnPosition(0.0f), Gun::Type::RIFLE, AMMO_RIFLE_PER_CRATE, assets));
  for (int i = 0; i < counts.pistolCrates; i++)
    ammoCrates.push_back(std::make_unique<AmmoCrate>(findSpawnPosition(0.0f), Gun::Type::PISTOL, AMMO_PISTOL_PER_CRATE, assets));
  for (int i = 0; i < counts.shotgunCrates; i++)
    ammoCrates.push_back(std::make_unique<AmmoCrate>(findSpawnPosition(0.0f), Gun::Type::SHOTGUN, AMMO_SHOTGUN_PER_CRATE, assets));
  for (int i = 0; i < counts.bandages; i++)
    bandages.push_back(std::make_unique<Bandage>(findSpawnPosition(0.0f), assets));
}

void World::spawnEnemy(EnemyType type, Vector2 position) {
  switch (type) {
    case EnemyType::ZOMBIE: {
      entities.push_back(std::make_unique<Zombie>(position, assets));
      Enemy* e = dynamic_cast<Enemy*>(entities.back().get());
      e->setTarget(player);
      enemies.push_back(e);
      break;
    }
    case EnemyType::GIANT: {
      entities.push_back(std::make_unique<Giant>(position, assets));
      Enemy* e = dynamic_cast<Enemy*>(entities.back().get());
      e->setTarget(player);
      enemies.push_back(e);
      break;
    }
    case EnemyType::GHOUL: {
      entities.push_back(std::make_unique<Ghoul>(position, assets));
      Ghoul* g = dynamic_cast<Ghoul*>(entities.back().get());
      g->setTarget(player);
      enemies.push_back(g);
      ghouls.push_back(g);
      break;
    }
  }
}

Vector2 World::findSpawnPosition(float minDistFromPlayer) {
  Vector2 playerPos = player ? player->getPosition() : Vector2{0, 0};
  Vector2 result = {SPAWN_AREA_X + SPAWN_AREA_W * 0.5f, SPAWN_AREA_Y + SPAWN_AREA_H * 0.5f};

  // Find a spawn position that is not blocked by collision boxes
  for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; ++attempt) {
    float x = static_cast<float>(GetRandomValue(
      static_cast<int>(SPAWN_AREA_X),
      static_cast<int>(SPAWN_AREA_X + SPAWN_AREA_W)
    ));
    float y = static_cast<float>(GetRandomValue(
      static_cast<int>(SPAWN_AREA_Y),
      static_cast<int>(SPAWN_AREA_Y + SPAWN_AREA_H)
    ));

    if (minDistFromPlayer > 0.0f) {
      float dx = x - playerPos.x;
      float dy = y - playerPos.y;
      if (sqrtf(dx * dx + dy * dy) < minDistFromPlayer) continue;
    }

    bool blocked = false;
    for (const Rectangle& box : collisionBoxes) {
      if (CheckPointInRect({x, y}, box)) { blocked = true; break; }
    }
    if (!blocked) return {x, y};
    result = {x, y};
  }
  return result;
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

void World::spawnBullets(Gun::Type type, Gun::Properties properties, int bulletCount, float spread) {
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
