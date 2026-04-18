#pragma once
#include "assets.h"
#include "items/equippable/melee.h"
#include "items/equippable/gun.h"

class Hotbar {
public:
  Hotbar(int screenWidth, int screenHeight, Assets& assets);

  void render(const Equippable* equipped) const;

private:
  int screenWidth;
  int screenHeight;
  Assets& assets;

  static constexpr int CELL_SIZE = 64;
  static constexpr int CELL_GAP = 4;
  static constexpr int NUM_SLOTS = 4;
  static constexpr float ICON_SCALE = 3.0f;
};
