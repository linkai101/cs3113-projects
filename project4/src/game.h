#pragma once
#include "raylib.h"
#include "entities/player.h"
#include "scenes/scene.h"
#include <memory>

class Game {
public:
  Game(int width, int height, const char* title);
  
  void init();

  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;
  float gPreviousTicks = 0.0f; // for calculating deltaTime

  Texture2D islandBgTexture;
  Texture2D islandTerrainTexture;
  Texture2D captainTexture;

  Spritesheet islandTerrainSheet;
  Spritesheet captainSheet;

  std::unique_ptr<Scene> level1;
  Scene* activeScene;

  std::unique_ptr<Player> player;

  void processInput();

  void update(float deltaTime);

  void render() const;

  void resetGame();

  void shutdown();
};