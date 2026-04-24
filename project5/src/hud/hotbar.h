#pragma once
#include "assets.h"
#include "items/equippable/melee.h"
#include "items/equippable/gun.h"

class Hotbar {
public:
  Hotbar(int screenWidth, int screenHeight, Assets& assets);

  void render(
    float health,
    float maxHealth,
    const Equippable* equipped,
    int rifleAmmo,
    int pistolAmmo,
    int shotgunAmmo
  ) const;

private:
  int screenWidth;
  int screenHeight;
  Assets& assets;

  void renderHealth(float health, float maxHealth) const;

  void renderCells(int activeSlot, int startX, int startY, int currentMag) const;

  void renderAmmoCounts(int rifleAmmo, int pistolAmmo, int shotgunAmmo) const;

  static constexpr int HEALTH_BAR_WIDTH = 180;
  static constexpr int HEALTH_BAR_HEIGHT = 16;
  static constexpr int HEALTH_FONT_SIZE = 20;

  static constexpr int CELL_SIZE = 80;
  static constexpr int CELL_GAP = 4;
  static constexpr int NUM_SLOTS = 4;
  static constexpr float ICON_SCALE = 4.0f;

  static constexpr int AMMO_ICON_WIDTH = 20;
  static constexpr int AMMO_ICON_MAX_HEIGHT = 48;
  static constexpr int AMMO_GAP = 20;
};
