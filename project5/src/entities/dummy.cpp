#include "entities/dummy.h"
#include "utils/color.h"
#include "raylib.h"
#include <string>
#include <cmath>

Dummy::Dummy(Vector2 spawnPosition, Assets& assets, float maxHealth) :
  Entity(spawnPosition, buildAnimator(&assets.playerSheet)),
  health(maxHealth), maxHealth(maxHealth)
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, true);
  playAnimation("idle-side");
}

void Dummy::update(float deltaTime) {
  // Update damage indicators
  for (auto& di : damageIndicators) {
    di.timer -= deltaTime;
    di.offset.y -= 30.0f * deltaTime;
  }
  damageIndicators.erase(
    std::remove_if(damageIndicators.begin(), damageIndicators.end(),
      [](const DamageIndicator& di) { return di.timer <= 0.0f; }),
    damageIndicators.end()
  );

  // Update based on state
  switch (state) {
    case State::ALIVE:
      // Update damage flash timer
      if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= deltaTime;
        if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
      }
    
      Entity::update(deltaTime);
      break;
    case State::DYING:
      if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= deltaTime;
        if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
      }
      Entity::update(deltaTime);

      // Begin death timer after animation is done
      if (animator.has_value() && animator->isAnimationDone()) {
        state = State::DEAD;
        deadTimer = DEAD_DURATION;
      }
      break;
    case State::DEAD:
      // Handle death timer
      deadTimer -= deltaTime;
      if (deadTimer <= 0.0f) respawn();
      break;
  };
}

void Dummy::render() const {
  Entity::render();

  // Render damage flash
  if (state != State::DEAD && damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    animator->render(position, ColorFromHex("#AA4A44", 0.5f * alpha));
  }

  // Render damage indicators
  for (const auto& di : damageIndicators) {
    float t = di.timer / DAMAGE_INDICATOR_DURATION;
    unsigned char alpha = static_cast<unsigned char>(255 * t);
    Color col = {255, 80, 80, alpha};

    std::string text = std::to_string(static_cast<int>(std::ceil(di.amount)));
    int fontSize = 20;
    int textW = MeasureText(text.c_str(), fontSize);
    float x = position.x + di.offset.x - textW / 2.0f;
    float y = position.y + di.offset.y;
    DrawText(text.c_str(), static_cast<int>(x), static_cast<int>(y), fontSize, col);
  }
}

void Dummy::takeDamage(float amount) {
  if (state != State::ALIVE) return;

  damageFlashTimer = DAMAGE_FLASH_DURATION;
  health -= amount;

  float xOffset = (float)(GetRandomValue(-15, 15));
  damageIndicators.push_back({amount, DAMAGE_INDICATOR_DURATION, {xOffset, -SIZE.y}});

  if (health <= 0.0f) {
    health = 0.0f;
    state = State::DYING;
    playAnimation("die");
  }
}

void Dummy::respawn() {
  health = maxHealth;
  state = State::ALIVE;
  playAnimation("idle-side");
}

Animator Dummy::buildAnimator(Spritesheet* sheet) {
  Animator anim(sheet, SIZE, Vector2{SIZE.x / 2, SIZE.y});

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 0, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 0, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 0, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  anim.addAnimation("attack-side", Animator::Animation{"attack-side", {54, 55, 56}, 10, false});
  anim.addAnimation("attack-down", Animator::Animation{"attack-down", {60, 61, 62}, 10, false});
  anim.addAnimation("attack-up", Animator::Animation{"attack-up", {66, 67, 68}, 10, false});
  anim.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 10, false});

  return anim;
}
