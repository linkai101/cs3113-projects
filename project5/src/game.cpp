#include "game.h"
#include "utils/log.h"
#include "utils/color.h"
#include "scenes/tutorial.h"
#include "scenes/main_menu.h"
#include "scenes/world.h"
#include "sound_manager.h"
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
  HideCursor();

  assets.load();
  SoundManager::get().init(&assets);

  tutorial = std::make_unique<Tutorial>(width, height, assets);
  mainMenu = std::make_unique<MainMenu>(width, height, assets);
  world = std::make_unique<World>(width, height, assets, "assets/levels/level1.txt");

  tutorial->load();
  activeScene = tutorial.get();

  shader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
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

  // Process input for active scene
  if (activeScene) activeScene->processInput();
}

void Game::update(float deltaTime) {
  SoundManager::get().update();
  if (activeScene) activeScene->update(deltaTime);

  if (activeScene) {
    SceneTransition t = activeScene->getTransition();
    if (t != SceneTransition::NONE) {
      switchScene(
        t == SceneTransition::TO_MENU ? mainMenu.get() : world.get()
      );
    }
  }
}

void Game::switchScene(Scene* next) {
  if (activeScene) activeScene->unload();
  activeScene = next;
  activeScene->load();
}

void Game::render() {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  if (activeScene) {
    shader.setFloat("damageFlash", activeScene->getPlayerDamageFlashIntensity());

    shader.begin();
    activeScene->render();
    shader.end();

    activeScene->renderHUD();
  }

  EndDrawing();
}

void Game::shutdown() {
  ShowCursor();
  shader.unload();
  assets.unload();

  CloseAudioDevice();
  CloseWindow();
}