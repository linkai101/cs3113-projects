#include "animated_entity.h"

AnimatedEntity::AnimatedEntity(
  Vector2 position,
  Vector2 size,
  Vector2 origin,
  const char* textureFilePath,
  Vector2 textureSpriteSize,
  int textureSpriteColumns,
  std::map<int, std::vector<int>> animationAtlas,
  int animationFPS,
  int idleAnimation
) :
  Entity(
    position,
    size,
    origin,
    0.0f, // rotation
    WHITE, // tint
    textureFilePath,
    textureSpriteSize,
    textureSpriteColumns,
    animationAtlas[idleAnimation][0] // textureSpriteIndex, use idle animation's first frame
  ),
  textureSpriteSize(textureSpriteSize),
  textureSpriteColumns(textureSpriteColumns),
  animationAtlas(std::move(animationAtlas)),
  animationFPS(animationFPS),
  idleAnimation(idleAnimation),
  currentAnimation(idleAnimation),
  currentAnimationFrameIndex(0),
  timeSinceLastFrameUpdate(0.0f),
  animationIsLooping(true)
{}

AnimatedEntity::~AnimatedEntity() {}

void AnimatedEntity::update(float deltaTime) {
  Entity::update(deltaTime);

  // Update current animation frame
  timeSinceLastFrameUpdate += deltaTime;
  if (timeSinceLastFrameUpdate >= 1.0f / animationFPS) {
    currentAnimationFrameIndex++;
    
    // Check if we've reached the end of the current animation
    if (currentAnimationFrameIndex >= animationAtlas.at(currentAnimation).size()) {
      if (animationIsLooping) {
        // Loop to start of the animation
        currentAnimationFrameIndex = 0;
      } else {
        // Non-looping animation, fall back to idle animation
        currentAnimation = idleAnimation;
        currentAnimationFrameIndex = 0;
        animationIsLooping = true;
      }
    }

    timeSinceLastFrameUpdate = 0.0f;
  }

  // Update textureArea for next animation frame
  int currentAnimationFrame = animationAtlas.at(currentAnimation).at(currentAnimationFrameIndex);
  setTextureArea(getSpritesheetSpriteTextureArea(textureSpriteSize, textureSpriteColumns, currentAnimationFrame));
}

void AnimatedEntity::render() const {
  Entity::render();
}

void AnimatedEntity::setIdleAnimation(int animation) {
  // If idle animation is already playing, play the new idle animation
  if (currentAnimation == idleAnimation) {
    playAnimation(animation, true);
  }

  // Set the new idle animation
  idleAnimation = animation;
}

void AnimatedEntity::playAnimation(int animation, bool loop) {
  // Skip if same animation is already playing and looping
  // Allows for replaying same animation for e.g. repeated attacks but maintaining same animation for e.g. walking
  if (animation == currentAnimation && animationIsLooping && loop) {
    return;
  }

  currentAnimation = animation;
  animationIsLooping = loop;
  currentAnimationFrameIndex = 0;
  timeSinceLastFrameUpdate = 0.0f;
}