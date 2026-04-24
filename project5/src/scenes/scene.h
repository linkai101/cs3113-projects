#pragma once
#include "assets.h"
#include "entities/entity.h"
#include "components/follow_camera.h"
#include <vector>

enum class SceneTransition { NONE, TO_MENU, TO_WORLD };

class Scene {
public:
  Scene(int screenWidth, int screenHeight, Assets& assets);
  virtual ~Scene() = default;

  virtual void load() { loaded = true; }

  virtual void unload() { loaded = false; }

  virtual void processInput() = 0;

  virtual void update(float deltaTime) = 0;

  virtual void render() const = 0;

  virtual void renderHUD() const {}

  virtual float getPlayerDamageFlashIntensity() const { return 0.0f; }

  virtual SceneTransition getTransition() const { return SceneTransition::NONE; }

protected:
  int screenWidth, screenHeight;
  Assets& assets;

  // Whether the scene is loaded
  bool loaded = false;
};