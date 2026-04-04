#include "scenes/main_menu.h"
#include "raylib.h"

MainMenu::MainMenu(int screenWidth, int screenHeight, Assets& assets)
  : Scene(screenWidth, screenHeight, {0, 0}, assets)
{}

void MainMenu::processInput(Player* player) {
  if (IsKeyPressed(KEY_ENTER)) {
    requestTransition();
  }
}

void MainMenu::update(float deltaTime, Player* player) {
  // Nothing to update on the main menu
}

void MainMenu::render(Player* player) const {
  const char* title = "High Tide Heist";
  const char* prompt = "Press ENTER to Play";

  int titleFontSize = 48;
  int promptFontSize = 20;

  int titleWidth = MeasureText(title, titleFontSize);
  int promptWidth = MeasureText(prompt, promptFontSize);

  DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 2 - 40, titleFontSize, WHITE);
  DrawText(prompt, (screenWidth - promptWidth) / 2, screenHeight / 2 + 30, promptFontSize, LIGHTGRAY);
}
