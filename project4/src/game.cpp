#include "game.h"
#include "scenes/level1.h"
#include "scenes/level2.h"
#include "utils/log.h"
#include "utils/color.h"
#include <memory>

Game::Game(
  int width,
  int height,
  const char* title
) :
  width(width),
  height(height),
  title(title),
  isRunning(true)
{}

void Game::init() {
  InitWindow(width, height, title);
  SetTargetFPS(60);

  // Load textures / spritesheets
  islandBgTexture = LoadTexture("assets/textures/island-bg.png");

  islandTerrainTexture = LoadTexture("assets/textures/island-terrain.png");
  islandTerrainSheet = Spritesheet{islandTerrainTexture, Vector2{ 32, 32 }, 17};

  smallMapTexture = LoadTexture("assets/textures/small-map.png");
  smallMapSheet = Spritesheet{smallMapTexture, Vector2{ 20, 20 }, 8};

  captainTexture = LoadTexture("assets/textures/captain.png");
  captainSheet = Spritesheet{captainTexture, Vector2{ 64, 40 }, 6};

  resetGame();
}

void Game::run() {
  while (isRunning) {
    processInput();

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    update(deltaTime);

    render();
  }

  shutdown();
}

void Game::processInput() {
  if (WindowShouldClose()) isRunning = false;

  // DEBUG: quick scene switching
  if (IsKeyPressed(KEY_ONE) && activeScene != level1.get()) {
    activeScene->unload();
    activeScene = level1.get();
    activeScene->load(player.get());
  } else if (IsKeyPressed(KEY_TWO) && activeScene != level2.get()) {
    activeScene->unload();
    activeScene = level2.get();
    activeScene->load(player.get());
  }

  // Process input for active scene
  if (activeScene) activeScene->processInput(player.get());
}

void Game::update(float deltaTime) {
  if (!player) return;

  // Update active scene
  if (activeScene) activeScene->update(deltaTime, player.get());

  // Handle level transition
  if (activeScene && activeScene->isTransitionRequested()) {
    activeScene->clearTransition();
    Scene* nextScene = nullptr;
    if (activeScene == level1.get()) nextScene = level2.get();
    if (nextScene) {
      activeScene->unload();
      activeScene = nextScene;
      activeScene->load(player.get());
    }
  }
}

void Game::render() const {
  BeginDrawing();

  ClearBackground(ColorFromHex("#33323D"));

  // Render active scene
  if (activeScene) activeScene->render(player.get());

  EndDrawing();
}

void Game::resetGame() {
  // Unload scene and player
  activeScene = nullptr;
  level1.reset();
  level2.reset();
  player.reset();

  // Create scenes
  level1 = std::make_unique<Level1>(
    width, height,
    islandTerrainSheet, smallMapSheet, islandBgTexture  
  );
  level2 = std::make_unique<Level2>(
    width, height,
    islandTerrainSheet, smallMapSheet, islandBgTexture
  );

  // Create player
  Animator playerAnimator = Animator(
    &captainSheet,
    Vector2{ 192, 120 }, // size
    Vector2{ 96, 60 } // origin
  );
  playerAnimator.addAnimation("idle", Animator::Animation{"idle", { 0, 1, 2, 3, 4 }, 10, true});
  playerAnimator.addAnimation("run", Animator::Animation{"run", { 6, 7, 8, 9, 10, 11 }, 10, true});
  playerAnimator.addAnimation("jump", Animator::Animation{"jump", { 12, 13, 14 }, 10, false});
  playerAnimator.addAnimation("fall", Animator::Animation{"fall", { 18 }, 10, true});
  playerAnimator.addAnimation("ground", Animator::Animation{"ground", { 24, 25 }, 10, false});
  playerAnimator.addAnimation("hit", Animator::Animation{"hit", { 30, 31, 32, 33 }, 10, false});
  playerAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", { 36, 37, 38, 39 }, 10, false});
  playerAnimator.addAnimation("dead-ground", Animator::Animation{"dead-ground", { 42, 43, 44, 45 }, 10, false});

  player = std::make_unique<Player>(
    Vector2{ width / 2.0f, height / 2.0f }, // spawnPosition
    playerAnimator
  );
  player->playAnimation("idle");

  player->enablePhysics(
    Vector2{ 50, 70 }, // colliderSize
    Vector2{ -25, -35 }, // colliderOffset
    false
  );

  // Load active scene
  activeScene = level1.get();
  activeScene->load(player.get());
}

void Game::shutdown() {
  CloseWindow();

  UnloadTexture(islandBgTexture);
  UnloadTexture(islandTerrainTexture);
  UnloadTexture(captainTexture);
}