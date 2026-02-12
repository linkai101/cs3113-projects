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

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int FPS = 60;

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;

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

void init() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1");
  SetTargetFPS(FPS);
}

void processInput() {
  if (WindowShouldClose()) {
    gAppStatus = TERMINATED;
  }
}

void update() {
  float ticks = (float) GetTime();
  float deltaTime = ticks - gPreviousTicks;
  gPreviousTicks = ticks;
}

void render() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  EndDrawing();
}

void shutdown() {
  CloseWindow();
}