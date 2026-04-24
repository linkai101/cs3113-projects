#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include "scenes/tutorial.h"
#include "components/sprite.h"
#include "sound_manager.h"
#include "utils/collision.h"

Tutorial::Tutorial(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, assets),
  camera(screenWidth, screenHeight),
  hotbar(screenWidth, screenHeight, assets)
{}

void Tutorial::load() {
  if (loaded) return;

  loadLevel("assets/levels/tutorial.txt");

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
  player->takeDamage(20); // Initial damage for tutorial

  // Spawn dummy
  entities.push_back(std::make_unique<Dummy>(getTilePosition(DUMMY_POSITION), assets));
  dummy = dynamic_cast<Dummy*>(entities.back().get());

  // Spawn ammo
  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({1.0f, 2.5f}), Gun::Type::RIFLE, 30, assets));
  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({1.0f, 3.5f}), Gun::Type::PISTOL, 12, assets));
  ammoCrates.push_back(std::make_unique<AmmoCrate>(getTilePosition({1.0f, 4.5f}), Gun::Type::SHOTGUN, 6, assets));

  // Spawn bandage
  bandages.push_back(std::make_unique<Bandage>(getTilePosition({1.0f, 1.5f}), assets));

  camera.init(player->getPosition());
  camera.setBounds(mapCols * TILE_SIZE, mapRows * TILE_SIZE);

  Scene::load();
}

void Tutorial::unload() {
  if (!loaded) return;

  terrain.clear();
  collisionBoxes.clear();
  entities.clear();
  bullets.clear();
  transition = SceneTransition::NONE;

  Scene::unload();
}

void Tutorial::processInput() {
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

void Tutorial::update(float deltaTime) {
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

  // Remove expired bullets
  bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
      [](const std::unique_ptr<Bullet>& b) { return b->isExpired(); }),
    bullets.end()
  );

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
  if (dummy) {
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

  // Trigger exit when player walks through the right-wall opening
  if (player && player->getPosition().x >= mapCols * TILE_SIZE) {
    transition = SceneTransition::TO_MENU;
  }
}

void Tutorial::render() const {
  if (!loaded) return;

  BeginMode2D(camera.get());

  // Render terrain entities
  for (auto& entity : terrain) {
    entity->render();
  }

  // Movement hint
  DrawText(
    "WASD to move",
    (SPAWN_POSITION.x) * TILE_SIZE - MeasureText("WASD to move", 20) / 2.0f, // posX
    (SPAWN_POSITION.y) * TILE_SIZE, // posY
    20,
    WHITE
  );

  // Bandage hint
  DrawText(
    "<- HEAL",
    (1.0f) * TILE_SIZE + 30, // posX
    1.5f * TILE_SIZE - 25, // posY
    20,
    WHITE
  );

  // Dummy hint
  DrawText(
    "YOUR FRIEND! :)",
    DUMMY_POSITION.x * TILE_SIZE - MeasureText("YOUR FRIEND! :)", 20) / 2.0f - 120, // posX
    DUMMY_POSITION.y * TILE_SIZE - 40, // posY
    20,
    WHITE
  );

  // Exit marker at the right-wall opening
  DrawText(
    "EXIT ->",
    (mapCols - 1.5f) * TILE_SIZE, // posX
    3.0f * TILE_SIZE - 14, // posY
    24,
    YELLOW
  );

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

  // Render bullets above all entities
  for (auto& bullet : bullets) bullet->render();

  EndMode2D();
}

void Tutorial::renderHUD() const {
  if (!loaded) return;

  if (player) {
    hotbar.render(
      player->getHealth(),
      Player::MAX_HEALTH,
      player->getEquipped(),
      player->getAmmoInventory(Gun::Type::RIFLE),
      player->getAmmoInventory(Gun::Type::PISTOL),
      player->getAmmoInventory(Gun::Type::SHOTGUN)
    );

    // Hotbar hints
    DrawText(
      "[Q] - HANDS",
      screenWidth / 2 - MeasureText("[Q] - HANDS", 20) - 200, // posX
      screenHeight - 36, // posY
      20,
      WHITE
    );
    DrawText(
      "[1]",
      screenWidth / 2 - MeasureText("[1]", 20) / 2.0f - 123, // posX
      screenHeight - 124, // posY
      20,
      WHITE
    );
    DrawText(
      "[2]",
      screenWidth / 2 - MeasureText("[2]", 20) / 2.0f - 42, // posX
      screenHeight - 124, // posY
      20,
      WHITE
    );
    DrawText(
      "[3]",
      screenWidth / 2 - MeasureText("[3]", 20) / 2.0f + 42, // posX
      screenHeight - 124, // posY
      20,
      WHITE
    );
    DrawText(
      "[4]",
      screenWidth / 2 - MeasureText("[4]", 20) / 2.0f + 123, // posX
      screenHeight - 124, // posY
      20,
      WHITE
    );
    DrawText(
      "[R] - RELOAD",
      screenWidth / 2 + 200, // posX
      screenHeight - 36, // posY
      20,
      WHITE
    );
  }
  
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

float Tutorial::getPlayerDamageFlashIntensity() const {
  return player ? player->getDamageFlashIntensity() : 0.0f;
}

void Tutorial::loadLevel(const std::string& path) {
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
      std::vector<int> gridRow;
      gridRow.push_back(std::stoi(token));
      int val;
      while (iss >> val) gridRow.push_back(val);
      currentGrid.push_back(std::move(gridRow));
    }
  }
  loadLayer();
}

void Tutorial::loadLayerFromGrid(const std::string& layerName, const std::vector<std::vector<int>>& grid) {
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

void Tutorial::loadTileGrid(
  std::vector<std::unique_ptr<Entity>>& vectorToAddTo,
  const int* grid, int rows, int cols,
  Spritesheet& sheet, Vector2 tileOffset,
  bool enablePhysics
) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int frameIndex = grid[r * cols + c];
      if (frameIndex == -1) continue;

      Vector2 position = getTilePosition({(float)c, (float)r}, tileOffset);
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

Vector2 Tutorial::getTilePosition(Vector2 tileCoords, Vector2 tileOffset) {
  return {
    (tileOffset.x + tileCoords.x) * TILE_SIZE,
    (tileOffset.y + tileCoords.y) * TILE_SIZE
  };
}

void Tutorial::spawnBullets(Gun::Type type, Gun::Properties properties, int bulletCount, float spread) {
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
