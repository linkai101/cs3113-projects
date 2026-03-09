#pragma once
#include "raylib.h"
#include "entity.h"
#include "player.h"

class Level {
public:
  Level(
    int screenWidth,
    int screenHeight
  );

  ~Level();

  virtual void processInput();

  virtual void update(float deltaTime);

  virtual void render();

private:
  int screenWidth;
  int screenHeight;
  std::vector<Entity*> environment;
  Player* player;

  /**
   * Creates a tile entity and adds it to the environment vector.
   * @param tileCoordinates The tile coordinates. Origin is the bottom-left corner, one unit is TILE_SIZE.
   * @param textureSpriteIndex The index of the texture sprite to use.
   */
  void createTile(Vector2 tileCoordinates, int textureSpriteIndex);
  
  // TODO: methods for creating grass and water entities

  static constexpr int TILE_SIZE = 48;

  /**
   * Calculates the position of a tile from its tile coordinates. Origin is the bottom-left corner.
   * @param tileCoordinates The tile coordinates. Origin is the bottom-left corner, one unit is TILE_SIZE.
   * @param screenWidth The width of the screen.
   * @param screenHeight The height of the screen.
   * @return The position of the tile.
   */
  static Vector2 getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight);
};