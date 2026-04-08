#pragma once
#include "assets.h"
#include "entities/entity.h"
#include "components/follow_camera.h"
#include <vector>

class Scene {
public:
  Scene(int screenWidth, int screenHeight, Assets& assets);
  virtual ~Scene() = default;

  virtual void load() { loaded = true; }

  virtual void unload() { loaded = false; }

  virtual void processInput() = 0;

  virtual void update(float deltaTime) = 0;

  virtual void render() const = 0;

protected:
  int screenWidth, screenHeight;
  Assets& assets;

  // Whether the scene is loaded
  bool loaded = false;
};