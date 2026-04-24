#pragma once
#include "raylib.h"
#include "assets.h"
#include "scenes/scene.h"
#include "utils/shader_program.h"
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

  Assets assets;

  std::unique_ptr<Scene> tutorial;
  std::unique_ptr<Scene> mainMenu;
  std::unique_ptr<Scene> world;
  Scene* activeScene;

  void switchScene(Scene* next);

  ShaderProgram shader;

  void processInput();

  void update(float deltaTime);

  void render();

  void shutdown();
};