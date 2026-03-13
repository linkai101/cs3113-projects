#include "game.h"
#include "chunks/tutorial_chunk.h"
#include "chunks/chunk1.h"
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
  skyTexture = LoadTexture("assets/textures/sky.png");
  mountains1Texture = LoadTexture("assets/textures/mountains_1.png");
  mountains2Texture = LoadTexture("assets/textures/mountains_2.png");

  tilesTexture = LoadTexture("assets/textures/tiles.png");
  tilesSheet = Spritesheet{tilesTexture, Vector2{ 16, 16 }, 4};

  grassTexture = LoadTexture("assets/textures/grass.png");
  grassSheet = Spritesheet{grassTexture, Vector2{ 16, 20 }, 9};

  waterTexture = LoadTexture("assets/textures/water.png");
  waterSheet = Spritesheet{waterTexture, Vector2{ 16, 16 }, 6};

  playerTexture = LoadTexture("assets/textures/jetpackman.png");
  playerSheet = Spritesheet{playerTexture, Vector2{ 692, 599 }, 5};

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
  if (WindowShouldClose()) {
    isRunning = false;
  }
  
  if (player->getGameState() == Player::PlayerGameState::PLAYING) {
    bool left = IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_D);
    bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE);

    player->move(left, right, up);
  } else {
    if (IsKeyPressed(KEY_ENTER)) {
      resetGame();
    }
  }

}

void Game::update(float deltaTime) {
  if (!player) return;

  if (player->getGameState() == Player::PlayerGameState::PLAYING && activeChunk) {
    activeChunk->update(deltaTime, player.get());

    // Resolve active chunk collisions
    activeChunk->resolveCollisions(player.get());

    // Check for chunk exits
    if (auto nextChunk = activeChunk->handleExit(player.get())) {
      if (nextChunk.has_value()) {
        activeChunk = nextChunk.value();
      }
    }
  }
}

void Game::render() const {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  // Render active chunk
  if (activeChunk) activeChunk->render(player.get());

  // Render fuel indicator
  float fuel = player->getFuel();
  float fuelMax = Player::FUEL_MAX;
  float fuelPercentage = fuel / fuelMax;
  DrawRectangle(width - 10 - 100, 10, 100, 12, ColorFromHex("#000000"));
  DrawRectangle(width - 10 - 100, 10, 100 * fuelPercentage, 12, ColorFromHex("#FFFFFF"));
  DrawText(
    "FUEL",
    width - 20 - 100 - MeasureText("FUEL", 15),
    10,
    15,
    ColorFromHex("#FFFFFF")
  );

  if (player->getGameState() == Player::PlayerGameState::DEAD) {
    DrawRectangle(0, 0, width, height, ColorFromHex("#000000", 0.8f));
    DrawText("YOU DIED", width / 2 - MeasureText("YOU DIED", 30) / 2, height / 2 - 30, 30, ColorFromHex("#FFFFFF"));
    DrawText("Press ENTER to restart", width / 2 - MeasureText("Press ENTER to restart", 15) / 2, height / 2 + 30, 15, ColorFromHex("#FFFFFF"));
  } else if (player->getGameState() == Player::PlayerGameState::WON) {
    DrawRectangle(0, 0, width, height, ColorFromHex("#000000", 0.8f));
    DrawText("YOU WON!", width / 2 - MeasureText("YOU WON!", 30) / 2, height / 2 - 30, 30, ColorFromHex("#FFFFFF"));
    DrawText("Press ENTER to restart", width / 2 - MeasureText("Press ENTER to restart", 15) / 2, height / 2 + 30, 15, ColorFromHex("#FFFFFF"));
  }

  EndDrawing();
}

void Game::resetGame() {
  // Unload chunks and player
  activeChunk = nullptr;
  tutorialChunk.reset();
  chunk1.reset();
  player.reset();

  // Create chunks
  tutorialChunk = std::make_unique<TutorialChunk>(
    width, height,
    tilesSheet, grassSheet, waterSheet,
    skyTexture, mountains1Texture, mountains2Texture
  );
  chunk1 = std::make_unique<Chunk1>(
    width, height,
    tilesSheet, grassSheet, waterSheet,
    skyTexture, mountains1Texture, mountains2Texture
  );
  
  tutorialChunk->setExit(Chunk::ExitDirection::EXIT_RIGHT, chunk1.get());
  chunk1->setExit(Chunk::ExitDirection::EXIT_LEFT, tutorialChunk.get());
  
  tutorialChunk->load();
  chunk1->load();
  activeChunk = tutorialChunk.get();

  // Create player
  Animator playerAnimator = Animator(
    &playerSheet,
    Vector2{ 150, 130 }, // size
    Vector2{ 90, 115 } // origin
  );
  playerAnimator.addAnimation("standing_idle", Animator::Animation{"standing_idle", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 10, true});
  playerAnimator.addAnimation("standing_walk", Animator::Animation{"standing_walk", { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }, 20, true});
  playerAnimator.addAnimation("standing_run", Animator::Animation{"standing_run", { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }, 30, true});
  playerAnimator.addAnimation("standing_jump", Animator::Animation{"standing_jump", { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 }, 15, false});
  playerAnimator.addAnimation("standing_die", Animator::Animation{"standing_die", { 55, 56, 57, 58, 59 }, 15, false});
  playerAnimator.addAnimation("flying", Animator::Animation{"flying", { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 }, 20, true});
  playerAnimator.addAnimation("flying_no_movement", Animator::Animation{"flying_no_movement", { 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 }, 20, true});
  playerAnimator.addAnimation("flying_die", Animator::Animation{"flying_die", { 85, 86, 87, 88, 89 }, 15, false});

  player = std::make_unique<Player>(
    Chunk::getPositionFromTileCoordinates({ 2, 5 }, width, height),
    playerAnimator
  );
  player->playAnimation("standing_idle");

  player->enablePhysics(
    Vector2{ 50, 110 }, // colliderSize
    Vector2{ -25, -110 }, // colliderOffset
    false
  );
}

void Game::shutdown() {
  CloseWindow();

  UnloadTexture(skyTexture);
  UnloadTexture(mountains1Texture);
  UnloadTexture(mountains2Texture);
  UnloadTexture(tilesTexture);
  UnloadTexture(grassTexture);
  UnloadTexture(waterTexture);
  UnloadTexture(playerTexture);
}