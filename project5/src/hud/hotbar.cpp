#include <algorithm>
#include <cmath>
#include "hud/hotbar.h"

Hotbar::Hotbar(int screenWidth, int screenHeight, Assets& assets) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  assets(assets)
{}

void Hotbar::render(float health, float maxHealth, const Equippable* equipped, int rifleAmmo, int pistolAmmo, int shotgunAmmo) const {
  const Melee* melee = dynamic_cast<const Melee*>(equipped);
  const Gun* gun = dynamic_cast<const Gun*>(equipped);

  int activeSlot = -1;
  if (melee && melee->getType() == Melee::Type::BAT) activeSlot = 0;
  else if (gun) {
    switch (gun->getType()) {
      case Gun::Type::RIFLE: activeSlot = 1; break;
      case Gun::Type::PISTOL: activeSlot = 2; break;
      case Gun::Type::SHOTGUN: activeSlot = 3; break;
    }
  }

  const int totalWidth = NUM_SLOTS * CELL_SIZE + (NUM_SLOTS - 1) * CELL_GAP;
  const int startX = (screenWidth - totalWidth) / 2;
  const int startY = screenHeight - CELL_SIZE - 16;

  renderHealth(health, maxHealth);
  renderCells(activeSlot, startX, startY, gun ? gun->getCurrentMag() : -1);
  renderAmmoCounts(rifleAmmo, pistolAmmo, shotgunAmmo);
}

void Hotbar::renderHealth(float health, float maxHealth) const {
  float ratio = health / maxHealth;
  const int barX = 16;
  const int bottomY = screenHeight - 16;
  const int barY = bottomY - HEALTH_BAR_HEIGHT;

  DrawText("HP", barX, barY - HEALTH_FONT_SIZE - 4, HEALTH_FONT_SIZE, WHITE);
  DrawRectangle(barX, barY, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, Color{80, 0, 0, 255});
  DrawRectangle(barX, barY, static_cast<int>(HEALTH_BAR_WIDTH * ratio), HEALTH_BAR_HEIGHT, Color{220, 50, 50, 255});
  DrawRectangleLines(barX, barY, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, WHITE);
}

void Hotbar::renderCells(int activeSlot, int startX, int startY, int currentMag) const {
  const Texture2D* pickableTextures[NUM_SLOTS] = {
    &assets.batPickableTexture,
    &assets.riflePickableTexture,
    &assets.pistolPickableTexture,
    &assets.shotgunPickableTexture,
  };

  for (int i = 0; i < NUM_SLOTS; i++) {
    int cellX = startX + i * (CELL_SIZE + CELL_GAP);

    // Render cell background
    const Texture2D& cellTex = (i == activeSlot) ? assets.hotbarCellSelectedTexture : assets.hotbarCellTexture;
    DrawTexturePro(
      cellTex,
      {0, 0, (float)cellTex.width, (float)cellTex.height},
      {(float)cellX, (float)startY, (float)CELL_SIZE, (float)CELL_SIZE},
      {0, 0},
      0.0f,
      WHITE
    );

    // Render pickable icon
    const Texture2D& pickable = *pickableTextures[i];
    float iconW = pickable.width * ICON_SCALE;
    float iconH = pickable.height * ICON_SCALE;
    DrawTexturePro(
      pickable,
      {0, 0, (float)pickable.width, (float)pickable.height},
      {(float)cellX + (CELL_SIZE - iconW) / 2.0f, (float)startY + (CELL_SIZE - iconH) / 2.0f, iconW, iconH},
      {0, 0},
      0.0f,
      WHITE
    );

    // Render mag count in active gun cell
    if (i == activeSlot && currentMag >= 0) {
      const int magFontSize = 20;
      std::string magText = std::to_string(currentMag);
      int textW = MeasureText(magText.c_str(), magFontSize);
      DrawText(magText.c_str(), cellX + CELL_SIZE - textW - 12, startY + CELL_SIZE - magFontSize - 10, magFontSize, WHITE);
    }
  }
}

void Hotbar::renderAmmoCounts(int rifleAmmo, int pistolAmmo, int shotgunAmmo) const {
  struct AmmoEntry { const Texture2D* icon; int count; };
  const AmmoEntry entries[3] = {
    { &assets.rifleBulletIconTexture, rifleAmmo },
    { &assets.pistolBulletIconTexture, pistolAmmo },
    { &assets.shotgunBulletIconTexture, shotgunAmmo },
  };

  const int fontSize = 20;
  const int bottomY = screenHeight - 16;

  int totalAmmoW = 0;
  for (int i = 0; i < 3; i++) {
    int textW = MeasureText(std::to_string(entries[i].count).c_str(), fontSize);
    totalAmmoW += AMMO_ICON_WIDTH + 4 + textW + (i < 2 ? AMMO_GAP : 0);
  }

  int currX = screenWidth - 16 - totalAmmoW;
  for (int i = 0; i < 3; i++) {
    const Texture2D& icon = *entries[i].icon;
    float iconH = std::min((float)AMMO_ICON_MAX_HEIGHT, AMMO_ICON_WIDTH / (float)icon.width * icon.height);
    float iconY = bottomY - iconH;

    // Render ammo icon
    DrawTexturePro(
      icon,
      {0, 0, (float)icon.width, (float)icon.height},
      {(float)currX, iconY, (float)AMMO_ICON_WIDTH, iconH},
      {0, 0},
      0.0f,
      WHITE
    );

    // Render ammo count text
    std::string countText = std::to_string(entries[i].count);
    int textW = MeasureText(countText.c_str(), fontSize);
    DrawText(countText.c_str(), currX + AMMO_ICON_WIDTH + 4, bottomY - fontSize, fontSize, WHITE);

    currX += AMMO_ICON_WIDTH + 4 + textW + AMMO_GAP;
  }
}
