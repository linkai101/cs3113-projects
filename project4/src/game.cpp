#include "game.h"
#include "scenes/main_menu.h"
#include "scenes/level1.h"
#include "scenes/level2.h"
#include "scenes/level3.h"
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
  InitAudioDevice();
  SetTargetFPS(60);

  assets.load();

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
  if (IsKeyPressed(KEY_GRAVE) && activeScene != mainMenu.get()) {
    activeScene->unload();
    activeScene = mainMenu.get();
  } else if (IsKeyPressed(KEY_ONE) && activeScene != level1.get()) {
    activeScene->unload();
    activeScene = level1.get();
    activeScene->load(player.get());
  } else if (IsKeyPressed(KEY_TWO) && activeScene != level2.get()) {
    activeScene->unload();
    activeScene = level2.get();
    activeScene->load(player.get());
  } else if (IsKeyPressed(KEY_THREE) && activeScene != level3.get()) {
    activeScene->unload();
    activeScene = level3.get();
    activeScene->load(player.get());
  }

  // Process input for active scene
  if (activeScene) activeScene->processInput(player.get());
}

void Game::update(float deltaTime) {
  if (!player) return;

  // Update active scene
  if (activeScene) activeScene->update(deltaTime, player.get());

  // Handle scene transition
  if (activeScene && activeScene->isTransitionRequested()) {
    activeScene->clearTransition();
    Scene* nextScene = nullptr;
    if (activeScene == mainMenu.get()) nextScene = level1.get();
    else if (activeScene == level1.get()) nextScene = level2.get();
    else if (activeScene == level2.get()) nextScene = level3.get();
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
  mainMenu.reset();
  level1.reset();
  level2.reset();
  level3.reset();
  player.reset();

  // Create scenes
  mainMenu = std::make_unique<MainMenu>(width, height, assets);
  level1 = std::make_unique<Level1>(width, height, assets);
  level2 = std::make_unique<Level2>(width, height, assets);
  level3 = std::make_unique<Level3>(width, height, assets);

  // Create player
  player = std::make_unique<Player>(
    Vector2{ width / 2.0f, height / 2.0f }, // spawnPosition
    assets
  );

  // Start at main menu
  activeScene = mainMenu.get();
}

void Game::shutdown() {
  assets.unload();

  CloseAudioDevice();
  CloseWindow();
}