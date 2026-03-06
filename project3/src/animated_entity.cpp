#include "animated_entity.h"

AnimatedEntity::AnimatedEntity(
  Vector2 position,
  Vector2 size,
  float rotation,
  Color tint,
  const char* textureFilePath,
  Vector2 textureFrameSize,
  int textureFrameColumns,
  std::map<int, std::vector<int>> animationAtlas,
  int animationFPS,
  int idleAnimation
) :
  Entity(
    position,
    size,
    rotation,
    tint,
    textureFilePath,
    getAnimationFrameTextureArea(textureFrameSize, textureFrameColumns, animationAtlas[idleAnimation][0]) // idle animation's first frame
  ),
  textureFrameSize(textureFrameSize),
  textureFrameColumns(textureFrameColumns),
  animationAtlas(animationAtlas),
  animationFPS(animationFPS),
  idleAnimation(idleAnimation),
  currentAnimation(idleAnimation),
  currentAnimationFrameIndex(0),
  timeSinceLastFrameUpdate(0.0f),
  animationIsLooping(true)
{}

AnimatedEntity::~AnimatedEntity() {}

void AnimatedEntity::init() {
  Entity::init();
}

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
}

void AnimatedEntity::render() {
  // Update textureArea for current animation frame
  int currentAnimationFrame = animationAtlas.at(currentAnimation).at(currentAnimationFrameIndex);
  textureArea = getAnimationFrameTextureArea(textureFrameSize, textureFrameColumns, currentAnimationFrame);

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

Rectangle AnimatedEntity::getAnimationFrameTextureArea(Vector2 frameSize, int frameColumns, int frame) {
  return {
    (frame % frameColumns) * frameSize.x,
    (frame / frameColumns) * frameSize.y,
    frameSize.x,
    frameSize.y
  };
}