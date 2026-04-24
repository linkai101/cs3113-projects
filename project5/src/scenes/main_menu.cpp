#include "scenes/main_menu.h"
#include "sound_manager.h"

MainMenu::MainMenu(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, assets)
{}

void MainMenu::load() {
  if (loaded) return;
  transition = SceneTransition::NONE;
  SoundManager::get().playMusic(MusicTrack::AMBIENT_WIND);
  Scene::load();
}

void MainMenu::unload() {
  if (!loaded) return;
  transition = SceneTransition::NONE;
  Scene::unload();
}

void MainMenu::processInput() {
  if (!loaded) return;
  if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
    transition = SceneTransition::TO_WORLD;
  }
}

void MainMenu::update(float) {}

void MainMenu::render() const {}

void MainMenu::renderHUD() const {
  if (!loaded) return;

  int cx = screenWidth / 2;

  const char* title = "HOLDOUT";
  int titleSize = 72;
  DrawText(title, cx - MeasureText(title, titleSize) / 2, screenHeight / 2 - 120, titleSize, WHITE);

  const char* subtitle = "A SURVIVAL GAME";
  int subtitleSize = 24;
  DrawText(subtitle, cx - MeasureText(subtitle, subtitleSize) / 2, screenHeight / 2 - 30, subtitleSize, LIGHTGRAY);

  const char* prompt = "Press ENTER to play";
  int promptSize = 24;
  DrawText(prompt, cx - MeasureText(prompt, promptSize) / 2, screenHeight / 2 + 100, promptSize, YELLOW);

  Vector2 mouse = GetMousePosition();
  DrawTexturePro(
    assets.cursorTexture,
    {0, 0, (float)assets.cursorTexture.width, (float)assets.cursorTexture.height},
    {mouse.x, mouse.y, 25.0f, 35.0f},
    {12.5f, 17.5f},
    0.0f,
    WHITE
  );
}
