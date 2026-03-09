#pragma once
#include "raylib.h"
#include "entity.h"
#include <map>
#include <vector>

class AnimatedEntity : public Entity {
public:
  AnimatedEntity(
    Vector2 position,
    Vector2 size,
    Vector2 origin,
    const char* textureFilePath,
    Vector2 textureSpriteSize,
    int textureSpriteColumns,
    std::map<int, std::vector<int>> animationAtlas,
    int animationFPS,
    int idleAnimation
  );

  ~AnimatedEntity();

  virtual void update(float deltaTime) override;

  virtual void render() const override;

  /**
   * Sets the idle animation to play when no other animation is playing.
   * @param animation The animation to set as idle. If the previous idle animation is already playing, the new idle animation will begin to play.
   */
  void setIdleAnimation(int animation);

  /**
   * Plays an animation.
   * @param animation The animation to play.
   * @param loop Whether to loop the animation. If false, the idle animation will be played after the animation finishes.
   */
  void playAnimation(int animation, bool loop = true);

private:
  Vector2 textureSpriteSize;
  int textureSpriteColumns;
  std::map<int, std::vector<int>> animationAtlas; // animation -> spritesheet sprite indices
  int animationFPS;
  int idleAnimation;
  int currentAnimation;
  int currentAnimationFrameIndex; // index within animationAtlas[currentAnimation]
  float timeSinceLastFrameUpdate;
  bool animationIsLooping;
};