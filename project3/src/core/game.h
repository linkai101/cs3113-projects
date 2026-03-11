#pragma once
#include "raylib.h"
#include "entities/player.h"
#include "chunks/chunk.h"
#include <memory>

class Game {
public:
  Game(int width, int height, const char* title);
  
  ~Game();

  void init();

  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;

  Texture2D skyTexture;
  Texture2D mountains1Texture;
  Texture2D mountains2Texture;
  Texture2D tilesTexture;
  Spritesheet tilesSheet;
  Texture2D grassTexture;
  Spritesheet grassSheet;
  Texture2D waterTexture;
  Spritesheet waterSheet;
  Texture2D playerTexture;
  Spritesheet playerSheet;

  std::unique_ptr<Chunk> tutorialChunk;
  std::unique_ptr<Chunk> chunk1;
  Chunk* activeChunk;

  std::unique_ptr<Player> player;

  void processInput();

  void update(float deltaTime);

  void render() const;
};