#pragma once
#include "chunk.h"

class TutorialChunk : public Chunk {
public:
  TutorialChunk(
    int screenWidth, int screenHeight,
    Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet,
    Texture2D& skyTexture, Texture2D& mountains1Texture, Texture2D& mountains2Texture
  );

  void resolveCollisions(Player* player) override;

  void render(Player* player) const override;

protected:
  void loadMap() override;

private:
  Texture2D skyTexture;
  Texture2D mountains1Texture;
  Texture2D mountains2Texture;
};