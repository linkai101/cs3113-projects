#pragma once
#include "raylib.h"
#include "components/spritesheet.h"
#include <unordered_map>
#include <string>
#include <vector>

class Animator {
public:
  struct Animation {
    std::string name;
    std::vector<int> frames; // indices into Spritesheet
    int fps;
    bool loop; // whether to loop the animation
  };

  Animator(
    Spritesheet* sheet,
    Vector2 size,
    Vector2 origin,
    bool flipX = false
  );

  /**
   * Adds an animation to the animator.
   * @param name The name of the animation. If an animation with this name already exists, the operation will be ignored.
   * @param animation The animation to add.
   */
  void addAnimation(const std::string& name, Animation animation);

  void play(const std::string& name);

  void update(float deltaTime);

  void setFlipX(bool flipX) { this->flipX = flipX; }
  bool getFlipX() const { return flipX; }

  std::string getCurrentAnimation() const { return currentAnimation; }

  bool isAnimationDone() const;
  bool isAnimationLooping() const;

  void render(Vector2 position) const;

private:
  Spritesheet* sheet;
  std::unordered_map<std::string, Animation> animations;
  std::string currentAnimation;
  int currentFrame = 0; // index into current animation's frames
  float timer = 0.0f;

  Vector2 size;
  Vector2 origin;
  bool flipX;
};