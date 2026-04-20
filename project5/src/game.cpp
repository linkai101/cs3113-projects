#include "game.h"
#include "utils/log.h"
#include "utils/color.h"
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

  assets.load();
  SoundManager::get().init(&assets);

  world = std::make_unique<World>(width, height, assets, "assets/levels/level1.txt");
  world->load();
  activeScene = world.get();

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
  shader.unload();
  assets.unload();

  CloseAudioDevice();
  CloseWindow();
}