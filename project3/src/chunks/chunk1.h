#pragma once
#include "chunk.h"

class Chunk1 : public Chunk {
public:
  Chunk1(
    int screenWidth, int screenHeight,
    Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet, Spritesheet& meteorSheet,
    Texture2D& skyTexture, Texture2D& mountains1Texture, Texture2D& mountains2Texture
  );

  void resolveCollisions(Player* player) override;

  void render(Player* player) const override;

protected:
  void loadMap() override;

private:
  Texture2D& skyTexture;
  Texture2D& mountains1Texture;
  Texture2D& mountains2Texture;
};