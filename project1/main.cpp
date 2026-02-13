/**
* Author: Linkai Wu
* Assignment: Simple 2D Scene
* Date due: 02/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>
#include <vector>

enum GameObject { PETER, APPLE, BOOK, CUP };

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int FPS = 60;

constexpr char PETER_FILEPATH[] = "assets/game/peter.png";
constexpr char APPLE_FILEPATH[] = "assets/game/apple.png";
constexpr char BOOK_FILEPATH[] = "assets/game/book.png";
constexpr char CUP_FILEPATH[] = "assets/game/cup.png";

constexpr float ORBIT_RADIUS_X = 180.0f;
constexpr float ORBIT_RADIUS_Y = 30.0f;
constexpr float ORBIT_DEGREES_PER_SECOND = 45.0f;
constexpr float ROTATION_DEGREES_PER_SECOND = 5.0f;
constexpr Vector2 PETER_SIZE = { 280.0f, 480.0f };
constexpr Vector2 APPLE_SIZE = { 60.0f, 60.0f };
constexpr Vector2 BOOK_SIZE = { 90.0f, 120.0f };
constexpr Vector2 CUP_SIZE = { 90.0f, 90.0f };

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;

Texture2D gPeterTexture;
Vector2 gPeterPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

Texture2D gAppleTexture;
Vector2 gAppleSizeScale = { 1.0f, 1.0f };
Vector2 gApplePosition = { 0, 0 }; // updated in update()
float gAppleRotation = 0.0f;
float gAppleThetaDegrees = 0.0f; // affects position in update()

Texture2D gBookTexture;
Vector2 gBookSizeScale = { 1.0f, 1.0f };
Vector2 gBookPosition = { 0, 0 }; // updated in update()
float gBookRotation = 0.0f;
float gBookThetaDegrees = 120.0f; // affects position in update()

Texture2D gCupTexture;
Vector2 gCupSizeScale = { 1.0f, 1.0f };
Vector2 gCupPosition = { 0, 0 }; // updated in update()
float gCupThetaDegrees = 240.0f; // affects position in update()

/**
 * Draws a game object
 */
void drawGameObject(GameObject gameObject);

/**
* Initializes the window and game loop
*/
void init();

/**
* Handles input
*/
void processInput();

/**
* Updates the game state
*/
void update();

/**
* Renders the game state
*/
void render();

/**
* Cleans up the window and game resources
*/
void shutdown();

/**
* Main function
*/
int main(void) {
  init();

  while (gAppStatus == RUNNING) {
    processInput();
    update();
    render();
  }

  shutdown();
  return 0;
}

void drawGameObject(GameObject gameObject) {
  switch (gameObject) {
    case PETER: {
      DrawTexturePro(
        gPeterTexture,
        { 0, 0, static_cast<float>(gPeterTexture.width), static_cast<float>(gPeterTexture.height) }, // texture area
        { gPeterPosition.x, gPeterPosition.y, PETER_SIZE.x, PETER_SIZE.y }, // destination area
        { PETER_SIZE.x / 2.0f, PETER_SIZE.y / 2.0f }, // object origin
        0.0f, // rotation
        WHITE // tint
      );
      break;
    }
    case APPLE: {
      Vector2 appleComputedSize = { APPLE_SIZE.x * gAppleSizeScale.x, APPLE_SIZE.y * gAppleSizeScale.y };
      DrawTexturePro(
        gAppleTexture,
        { 0, 0, static_cast<float>(gAppleTexture.width), static_cast<float>(gAppleTexture.height) }, // texture area
        { gApplePosition.x, gApplePosition.y, appleComputedSize.x, appleComputedSize.y }, // destination area
        { appleComputedSize.x / 2.0f, appleComputedSize.y / 2.0f }, // object origin
        gAppleRotation, // rotation
        WHITE // tint
      );
      break;
    }
    case BOOK: {
      Vector2 bookComputedSize = { BOOK_SIZE.x * gBookSizeScale.x, BOOK_SIZE.y * gBookSizeScale.y };
      DrawTexturePro(
        gBookTexture,
        { 0, 0, static_cast<float>(gBookTexture.width), static_cast<float>(gBookTexture.height) }, // texture area
        { gBookPosition.x, gBookPosition.y, bookComputedSize.x, bookComputedSize.y }, // destination area
        { bookComputedSize.x / 2.0f, bookComputedSize.y / 2.0f }, // object origin
        gBookRotation, // rotation
        WHITE // tint
      );
      break;
    }
    case CUP: {
      Vector2 cupComputedSize = { CUP_SIZE.x * gCupSizeScale.x, CUP_SIZE.y * gCupSizeScale.y };
      DrawTexturePro(
        gCupTexture,
        { 0, 0, static_cast<float>(gCupTexture.width), static_cast<float>(gCupTexture.height) }, // texture area
        { gCupPosition.x, gCupPosition.y, cupComputedSize.x, cupComputedSize.y }, // destination area
        { cupComputedSize.x / 2.0f, cupComputedSize.y / 2.0f }, // object origin
        0.0f, // rotation
        WHITE // tint
      );
      break;
    }
  }
}

void init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Peter's Orbit");
  SetTargetFPS(FPS);

  // Load textures
  gPeterTexture = LoadTexture(PETER_FILEPATH);
  gAppleTexture = LoadTexture(APPLE_FILEPATH);
  gBookTexture = LoadTexture(BOOK_FILEPATH);
  gCupTexture = LoadTexture(CUP_FILEPATH);
}

void processInput() {
  if (WindowShouldClose()) {
    gAppStatus = TERMINATED;
  }
}

void update() {
  // Calculate delta time
  float ticks = (float) GetTime();
  float deltaTime = ticks - gPreviousTicks;
  gPreviousTicks = ticks;

  // Update apple
  gAppleThetaDegrees = fmodf(gAppleThetaDegrees + deltaTime * ORBIT_DEGREES_PER_SECOND, 360.0f);
  gApplePosition.x = SCREEN_WIDTH / 2 + ORBIT_RADIUS_X * cosf(gAppleThetaDegrees * M_PI / 180.0f);
  gApplePosition.y = SCREEN_HEIGHT / 2 + ORBIT_RADIUS_Y * sinf(gAppleThetaDegrees * M_PI / 180.0f);
  gAppleRotation = fmodf(gAppleRotation + deltaTime * ROTATION_DEGREES_PER_SECOND, 360.0f);
  gAppleSizeScale.x = 1.0f + 0.2f * sinf(gAppleThetaDegrees * M_PI / 180.0f);
  gAppleSizeScale.y = 1.0f + 0.2f * sinf(gAppleThetaDegrees * M_PI / 180.0f);

  // Update book
  gBookThetaDegrees = fmodf(gBookThetaDegrees + deltaTime * ORBIT_DEGREES_PER_SECOND, 360.0f);
  gBookPosition.x = SCREEN_WIDTH / 2 + ORBIT_RADIUS_X * cosf(gBookThetaDegrees * M_PI / 180.0f);
  gBookPosition.y = SCREEN_HEIGHT / 2 + ORBIT_RADIUS_Y * sinf(gBookThetaDegrees * M_PI / 180.0f);
  gBookRotation = fmodf(gBookRotation + deltaTime * ROTATION_DEGREES_PER_SECOND, 360.0f);
  gBookSizeScale.x = 1.0f + 0.2f * sinf(gBookThetaDegrees * M_PI / 180.0f);
  gBookSizeScale.y = 1.0f + 0.2f * sinf(gBookThetaDegrees * M_PI / 180.0f);

  // Update cup
  gCupThetaDegrees = fmodf(gCupThetaDegrees + deltaTime * ORBIT_DEGREES_PER_SECOND, 360.0f);
  gCupPosition.x = SCREEN_WIDTH / 2 + ORBIT_RADIUS_X * cosf(gCupThetaDegrees * M_PI / 180.0f);
  gCupPosition.y = SCREEN_HEIGHT / 2 + ORBIT_RADIUS_Y * sinf(gCupThetaDegrees * M_PI / 180.0f);
  gCupSizeScale.x = 1.0f + 0.2f * sinf(gCupThetaDegrees * M_PI / 180.0f);
  gCupSizeScale.y = 1.0f + 0.2f * sinf(gCupThetaDegrees * M_PI / 180.0f);
}

void render() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  // Determine game object draw order
  std::vector<GameObject> objectsBehindPeter;
  std::vector<GameObject> objectsInFrontOfPeter = {};
  if (gAppleThetaDegrees > 180.0f) {
    objectsBehindPeter.push_back(APPLE);
  } else {
    objectsInFrontOfPeter.push_back(APPLE);
  }
  if (gBookThetaDegrees > 180.0f) {
    objectsBehindPeter.push_back(BOOK);
  } else {
    objectsInFrontOfPeter.push_back(BOOK);
  }
  if (gCupThetaDegrees > 180.0f) {
    objectsBehindPeter.push_back(CUP);
  } else {
    objectsInFrontOfPeter.push_back(CUP);
  }

  // Draw objects behind Peter
  for (GameObject object : objectsBehindPeter) {
    drawGameObject(object);
  }

  // Draw Peter
  drawGameObject(PETER);

  // Draw objects in front of Peter
  for (GameObject object : objectsInFrontOfPeter) {
    drawGameObject(object);
  }

  EndDrawing();
}

void shutdown() {
  CloseWindow();
}