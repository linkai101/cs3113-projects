#include "entity.h"

Entity::Entity(
  Vector2 position,
  Vector2 size,
  float rotation,
  Color tint,
  const char* textureFilePath,
  std::optional<Rectangle> textureArea
) :
  textureFilePath(textureFilePath),
  textureArea(textureArea),
  position(position),
  size(size),
  rotation(rotation),
  tint(tint)
{
  init();
}

Entity::~Entity() {
  UnloadTexture(texture);
}

Rectangle Entity::getRect() const {
  return { position.x - size.x / 2.0f, position.y - size.y / 2.0f, size.x, size.y };
}

float Entity::getRotation() const {
  return rotation;
}

void Entity::init() {
  texture = LoadTexture(textureFilePath);
}

void Entity::update(float deltaTime) {}

void Entity::render() {
  Rectangle destinationArea = { position.x, position.y, size.x, size.y };
  Vector2 origin = { size.x / 2.0f, size.y / 2.0f };

  DrawTexturePro(
    texture,
    textureArea.value_or(Rectangle{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) }),
    destinationArea,
    origin,
    rotation,
    tint
  );
}