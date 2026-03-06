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
    float rotation,
    Color tint,
    const char* textureFilePath,
    Vector2 textureFrameSize,
    int textureFrameColumns,
    std::map<int, std::vector<int>> animationAtlas,
    int animationFPS,
    int idleAnimation
  );

  ~AnimatedEntity();

  virtual void init() override;

  virtual void update(float deltaTime) override;

  virtual void render() override;

  void setIdleAnimation(int animation);

  /**
   * Plays an animation.
   * @param animation The animation to play.
   * @param loop Whether to loop the animation. If false, the idle animation will be played after the animation finishes.
   */
  void playAnimation(int animation, bool loop = true);

private:
  Vector2 textureFrameSize;
  int textureFrameColumns;
  std::map<int, std::vector<int>> animationAtlas; // animation -> animation frames
  int animationFPS;
  int idleAnimation;
  int currentAnimation;
  int currentAnimationFrameIndex;
  float timeSinceLastFrameUpdate;
  bool animationIsLooping;

  static Rectangle getAnimationFrameTextureArea(Vector2 frameSize, int frameColumns, int frame);
};