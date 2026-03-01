#pragma once
#include "raylib.h"
#include <optional>

/**
 * Abstract class for all textured renderable entities in the game.
 */
class Entity {
public:
  /**
   * Constructor for Entity.
   * @param position The position of the entity.
   * @param size The size of the entity.
   * @param rotation The rotation of the entity.
   * @param tint The tint of the entity. Use WHITE for no tint.
   * @param textureFilePath The path to the texture file.
   * @param textureArea The area of the texture to render. If not provided, the entire texture will be used.
   */
  Entity(
    Vector2 position,
    Vector2 size,
    float rotation,
    Color tint,
    const char* textureFilePath,
    std::optional<Rectangle> textureArea = std::nullopt
  );

  ~Entity();

  Rectangle getRect() const;

  float getRotation() const;

  virtual void init();

  virtual void update(float deltaTime);

  virtual void render();

protected:
  Vector2 position;
  Vector2 size;
  float rotation;
  Color tint;
  std::optional<Rectangle> textureArea;

private:
  const char* textureFilePath;
  Texture2D texture;
};