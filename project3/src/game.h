#pragma once
#include "raylib.h"
#include "entities/player.h"
#include "chunks/chunk.h"
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

  Texture2D skyTexture;
  Texture2D mountains1Texture;
  Texture2D mountains2Texture;
  Texture2D tilesTexture;
  Texture2D grassTexture;
  Texture2D waterTexture;
  Texture2D playerTexture;
  Spritesheet playerSheet;

  Spritesheet tilesSheet;
  Spritesheet grassSheet;
  Spritesheet waterSheet;

  std::unique_ptr<Chunk> tutorialChunk;
  std::unique_ptr<Chunk> chunk1;
  Chunk* activeChunk;

  std::unique_ptr<Player> player;

  void processInput();

  void update(float deltaTime);

  void render() const;

  void resetGame();

  void shutdown();
};