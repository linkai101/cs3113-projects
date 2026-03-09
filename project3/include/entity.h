#pragma once
#include "raylib.h"
#include <optional>

/**
 * Class for all textured renderable entities in the game.
 */
class Entity {
public:
  /**
   * Constructor for Entity, specifying a specific texture area.
   * @param position The position of the entity.
   * @param size The size of the entity.
   * @param origin The origin of the entity.
   * @param textureFilePath The path to the texture file.
   * @param textureArea The area of the texture to render. If not provided, the entire texture will be used.
   */
  Entity(
    Vector2 position,
    Vector2 size,
    Vector2 origin,
    const char* textureFilePath,
    std::optional<Rectangle> textureArea = std::nullopt
  );

  /**
   * Constructor for Entity, specifying a specific sprite from a spritesheet.
   * @param position The position of the entity.
   * @param size The size of the entity.
   * @param origin The origin of the entity.
   * @param textureFilePath The path to the texture file.
   * @param textureSpriteSize The size of the sprite in the spritesheet.
   * @param textureSpriteColumns The number of columns in the spritesheet.
   * @param textureSpriteIndex The index of the sprite in the spritesheet.
   */
  Entity(
    Vector2 position,
    Vector2 size,
    Vector2 origin,
    const char* textureFilePath,
    Vector2 textureSpriteSize,
    int textureSpriteColumns,
    int textureSpriteIndex
  );

  ~Entity();

  // Prevent copying and assignment
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;

  virtual void update(float deltaTime);

  virtual void render() const;

  void setFlipX(bool flipX) { this->flipX = flipX; }

protected:
  // void setPosition(Vector2 position) { this->position = position; }

  // void setSize(Vector2 size) { this->size = size; }

  // void setOrigin(Vector2 origin) { this->origin = origin; }

  // void setRotation(float rotation) { this->rotation = rotation; }

  // void setTint(Color tint) { this->tint = tint; }

  void setTextureArea(std::optional<Rectangle> textureArea) { this->textureArea = textureArea; }

  /**
   * Calculates the texture area of a sprite from a spritesheet.
   * @param spriteSize The size of the sprite.
   * @param spriteColumns The number of columns in the spritesheet.
   * @param spriteIndex The index of the sprite in the spritesheet.
   * @return The texture area of the sprite.
   */
  static Rectangle getSpritesheetSpriteTextureArea(Vector2 spriteSize, int spriteColumns, int spriteIndex);

private:
  const char* textureFilePath;
  Texture2D texture;

  Vector2 position;
  Vector2 size;
  Vector2 origin;
  float rotation = 0.0f;
  Color tint = WHITE;
  std::optional<Rectangle> textureArea;
  bool flipX = false;
};