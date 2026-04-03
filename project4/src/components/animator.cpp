#include "components/animator.h"

Animator::Animator(
  Spritesheet* sheet,
  Vector2 size,
  Vector2 origin,
  bool flipX
) :
  sheet(sheet),
  size(size),
  origin(origin),
  flipX(flipX)
{}

void Animator::addAnimation(const std::string& name, Animation animation) {
  animations.insert({ name, animation });
}

void Animator::play(const std::string& name) {
  if (animations.find(name) != animations.end()) {
    currentAnimation = name;
    currentFrame = 0;
    timer = 0.0f;
  }
}

void Animator::update(float deltaTime) {
  timer += deltaTime;

  // Check if next frame is due
  if (!currentAnimation.empty()) {
    float frameDuration = 1.0f / animations.at(currentAnimation).fps;
    if (timer >= frameDuration) {
      currentFrame++;

      // Check if end of animation
      if (currentFrame >= animations.at(currentAnimation).frames.size()) {
        if (animations.at(currentAnimation).loop) {
          // Loop to the start
          currentFrame = 0;
        } else {
          // Non-looping animation, stay on the last frame
          currentFrame = animations.at(currentAnimation).frames.size() - 1;
        }
      }

      timer -= frameDuration;
    }
  }
}

void Animator::render(Vector2 position) const {
  if (!currentAnimation.empty()) {
    Rectangle sourceArea = sheet->getFrame(animations.at(currentAnimation).frames[currentFrame]);
    if (flipX) sourceArea.width = -sourceArea.width; // Apply flipX to source area

    Rectangle destinationArea = { position.x, position.y, size.x, size.y };

    Vector2 originActual = flipX ? Vector2{ size.x - origin.x, origin.y } : origin; // Apply flipX to origin

    DrawTexturePro(
      sheet->texture,
      sourceArea,
      destinationArea,
      originActual,
      0.0f, // rotation
      WHITE // tint
    );
  }

  // DEBUG: rendered bounds
  // DrawRectangleLines(position.x - origin.x, position.y - origin.y, size.x, size.y, RED);

  // DEBUG: origin
  // DrawCircle(position.x, position.y, 2, RED);
}