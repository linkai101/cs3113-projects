#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "components/animator.h"

class Player : public Entity {
public:
  enum PlayerGameState { PLAYING, DEAD, WON };

  Player(Vector2 spawnPosition, Animator animator);

  void update(float deltaTime) override;

  void render() const override;

  void playAnimation(const std::string& animationName);

  void move(bool left, bool right, bool up);

  PlayerGameState getGameState() const { return gameState; }

  void setGameState(PlayerGameState gameState) { this->gameState = gameState; }
  
  float getFuel() const { return fuel; }

  void setJumping(bool jumping) { this->jumping = jumping; }
  
  void setCanBoost(bool canBoost) { this->canBoost = canBoost; }

  void setBoosting(bool boosting) { this->boosting = boosting; }

  bool isBoosting() const { return boosting; }

  static constexpr float FUEL_MAX = 15.0f;

private:
  PlayerGameState gameState = PlayerGameState::PLAYING;
  float fuel;

  bool movingLeft = false;
  bool movingRight = false;
  bool movingUp = false;

  bool canJump = true;
  bool jumping = false;
  bool canBoost = false; // can activate jetpack
  bool boosting = false; // using jetpack

  static constexpr float FUEL_CONSUMPTION_PER_SECOND = 1.0f;

  static constexpr float GRAVITY_ACCELERATION = 1200.0f;
  static constexpr float GROUND_VELOCITY = 250.0f;
  static constexpr float JUMP_INITIAL_VELOCITY = 350.0f;
  static constexpr float BOOST_Y_ACCELERATION = 600.0f;
  static constexpr float BOOST_X_ACCELERATION = 300.0f;
  static constexpr float BOOST_X_MAX_VELOCITY = 250.0f;
  static constexpr float GLIDE_X_ACCELERATION = 150.0f;
  
};