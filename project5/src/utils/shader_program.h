#pragma once
#include "raylib.h"
#include <string>

class ShaderProgram {
public:
  static constexpr int NOT_LOADED = -1;

  ShaderProgram();
  ~ShaderProgram();

  bool load(const std::string& vertexPath, const std::string& fragmentPath);
  void unload();

  void begin();
  void end();

  void setVector2(const std::string& name, const Vector2& value);
  void setFloat(const std::string& name, float value);
  void setInt(const std::string& name, int value);

  bool isLoaded() const { return mIsLoaded; }

private:
  Shader mShader;
  bool mIsLoaded = false;
};
