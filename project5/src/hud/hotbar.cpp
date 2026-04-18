#include <algorithm>
#include <cmath>
#include "hud/hotbar.h"

Hotbar::Hotbar(int screenWidth, int screenHeight, Assets& assets) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  assets(assets)
{
  const Texture2D* pickableTextures[NUM_SLOTS] = {
    &assets.batPickableTexture,
    &assets.riflePickableTexture,
    &assets.pistolPickableTexture,
    &assets.shotgunPickableTexture,
  };
}

void Hotbar::render(const Equippable* equipped) const {
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

  const Texture2D* pickableTextures[NUM_SLOTS] = {
    &assets.batPickableTexture,
    &assets.riflePickableTexture,
    &assets.pistolPickableTexture,
    &assets.shotgunPickableTexture,
  };

  for (int i = 0; i < NUM_SLOTS; i++) {
    int cellX = startX + i * (CELL_SIZE + CELL_GAP);

    const Texture2D& cellTex = (i == activeSlot) ? assets.hotbarCellSelectedTexture : assets.hotbarCellTexture;
    Rectangle src = {0, 0, (float)cellTex.width, (float)cellTex.height};
    Rectangle dst = {(float)cellX, (float)startY, (float)CELL_SIZE, (float)CELL_SIZE};
    DrawTexturePro(cellTex, src, dst, {0, 0}, 0.0f, WHITE);

    const Texture2D& pickable = *pickableTextures[i];
    float iconW = pickable.width * ICON_SCALE;
    float iconH = pickable.height * ICON_SCALE;
    float iconX = cellX + (CELL_SIZE - iconW) / 2.0f;
    float iconY = startY + (CELL_SIZE - iconH) / 2.0f;
    Rectangle iconSrc = {0, 0, (float)pickable.width, (float)pickable.height};
    Rectangle iconDst = {iconX, iconY, iconW, iconH};
    DrawTexturePro(pickable, iconSrc, iconDst, {0, 0}, 0.0f, WHITE);
  }
}
