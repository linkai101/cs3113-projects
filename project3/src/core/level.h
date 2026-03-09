#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "entities/player.h"
#include <memory>
#include <vector>

class Level {
public:
  Level(int screenWidth, int screenHeight);

  ~Level();

  virtual void processInput();

  virtual void update(float deltaTime);

  virtual void render() const;

private:
  int screenWidth;
  int screenHeight;

  Texture2D skyTexture;
  Texture2D mountains1Texture;
  Texture2D mountains2Texture;
  Texture2D tilesTexture;
  Spritesheet tilesSheet;
  Texture2D playerTexture;
  Spritesheet playerSheet;

  std::vector<std::unique_ptr<Entity>> environment;
  std::unique_ptr<Player> player;

  void createBackgroundLayer(Texture2D texture);
  
  void createTile(Vector2 tileCoordinates, int frameIndex);
  
  // TODO: methods for creating grass and water entities

  static constexpr int TILE_SIZE = 48;
  
  enum TileType {
    BLOCK = 0,
    INNER = 1,
    INNER_FOSSIL = 2,
    TOP_ALT = 3,
    TOP = 4,
    BOTTOM = 5,
    LEFT = 6,
    RIGHT = 7,
    TOP_LEFT = 8,
    TOP_RIGHT = 9,
    BOTTOM_LEFT = 10,
    BOTTOM_RIGHT = 11,
    CORNER_TOP_LEFT = 12,
    CORNER_TOP_RIGHT = 13,
    CORNER_BOTTOM_LEFT = 14,
    CORNER_BOTTOM_RIGHT = 15,
    SLOPE_TOP_LEFT = 16,
    SLOPE_TOP_RIGHT = 17,
    BRIDGE_LEFT = 18,
    BRIDGE_RIGHT = 19,
    BRIDGE_MIDDLE = 20
  };

  /**
   * Calculates the position of a tile from its tile coordinates. Origin is the bottom-left corner.
   * @param tileCoordinates The tile coordinates. Origin is the bottom-left corner, one unit is TILE_SIZE.
   * @param screenWidth The width of the screen.
   * @param screenHeight The height of the screen.
   * @return The position of the tile.
   */
  static Vector2 getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight);
};