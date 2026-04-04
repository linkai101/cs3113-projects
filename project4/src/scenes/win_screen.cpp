#include "scenes/win_screen.h"
#include "raylib.h"

WinScreen::WinScreen(int screenWidth, int screenHeight, Assets& assets)
  : Scene(screenWidth, screenHeight, {0, 0}, assets)
{}

void WinScreen::processInput(Player* player) {
  if (IsKeyPressed(KEY_ENTER)) {
    requestTransition();
  }
}

void WinScreen::update(float deltaTime, Player* player) {
  // Nothing to update
}

void WinScreen::render(Player* player) const {
  const char* title = "You Win!";
  const char* prompt = "Press ENTER to return to Main Menu";

  int titleFontSize = 36;
  int promptFontSize = 20;

  int titleWidth = MeasureText(title, titleFontSize);
  int promptWidth = MeasureText(prompt, promptFontSize);

  DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 2 - 34, titleFontSize, YELLOW);
  DrawText(prompt, (screenWidth - promptWidth) / 2, screenHeight / 2 + 30, promptFontSize, LIGHTGRAY);
}
