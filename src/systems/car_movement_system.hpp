#ifndef CAR_MOVEMENT_SYSTEM_HPP
#define CAR_MOVEMENT_SYSTEM_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include "../ECS/ECS.hpp"
#include "../components/nitro_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"

/**
 * @brief Controls the player car using angle-based top-down movement.
 *
 * Each frame it:
 * 1. Reads turn_left / turn_right input and rotates TransformComponent.rotation.
 * 2. If "accelerate" is held, computes the target velocity from the car's
 *    current angle and applies it immediately.  Nitro multiplies base speed by
 *    NITRO_MULTIPLIER (2.5×) — visually very noticeable.
 * 3. If "accelerate" is released, applies exponential velocity decay so the car
 *    slides to a stop instead of stopping instantly (BRAKE_FACTOR controls how
 *    fast it decelerates — lower = more friction, higher = longer slide).
 * 4. Decrements the jump timer.
 *
 * The MovementSystem (existing) applies velocity × deltaTime to the position.
 *
 * Coordinate convention (SDL top-left origin):
 *   rotation=0   → facing north  (vx=0, vy=-speed)
 *   rotation=90  → facing east   (vx=+speed, vy=0)
 */
class CarMovementSystem : public System {
 public:
  CarMovementSystem() {
    requireComponent<PlayerComponent>();
    requireComponent<TransformComponent>();
    requireComponent<RigidBodyComponent>();
    requireComponent<NitroComponent>();
  }

  /**
   * @brief Update car rotation and compute velocity from input.
   * @param deltaTime Seconds elapsed since the last frame
   */
  void update(double deltaTime) {
    auto& ctrl = *Game::getInstance().controllerManager;
    const float dt = static_cast<float>(deltaTime);

    for (Entity entity : getEntities()) {
      auto& player    = entity.getComponent<PlayerComponent>();
      auto& transform = entity.getComponent<TransformComponent>();
      auto& body      = entity.getComponent<RigidBodyComponent>();
      const auto& nitro = entity.getComponent<NitroComponent>();

      if (player.isDead || player.hasWon) {
        body.velocity = {0.0f, 0.0f};
        continue;
      }

      // ── Jump timer + scale effect ─────────────────────────────────────────
      if (player.isJumping) {
        player.jumpTimer -= dt;
        if (player.jumpTimer <= 0.0f) {
          // Landing: restore the entity's resting scale
          player.isJumping = false;
          player.jumpTimer = 0.0f;
          transform.scale  = {player.baseScaleX, player.baseScaleY};
        } else {
          // Sine-wave scale relative to base: peaks at jumpMaxScale × base.
          // sin curve ensures smooth take-off and landing transitions.
          const float progress = 1.0f - (player.jumpTimer / player.jumpDuration);
          const float extra    = (player.jumpMaxScale - 1.0f)
                                 * std::sin(progress * static_cast<float>(M_PI));
          transform.scale = {player.baseScaleX * (1.0f + extra),
                             player.baseScaleY * (1.0f + extra)};
        }
      }

      // ── Rotation ──────────────────────────────────────────────────────────
      if (ctrl.isActionActivated("turn_left"))
        transform.rotation -= player.rotationSpeed * deltaTime;
      if (ctrl.isActionActivated("turn_right"))
        transform.rotation += player.rotationSpeed * deltaTime;

      while (transform.rotation <   0.0) transform.rotation += 360.0;
      while (transform.rotation >= 360.0) transform.rotation -= 360.0;

      // ── Forward movement / braking ────────────────────────────────────────
      if (ctrl.isActionActivated("accelerate")) {
        // Nitro gives a very noticeable 2.5× speed burst
        float speed = player.baseSpeed * (nitro.isBoosting ? NITRO_MULTIPLIER : 1.0f);
        const double rad = transform.rotation * M_PI / 180.0;
        body.velocity.x = speed * static_cast<float>(std::sin(rad));
        body.velocity.y = speed * static_cast<float>(-std::cos(rad));
      } else {
        // Exponential decay: car slides to a stop instead of halting instantly.
        // pow(BRAKE_FACTOR, dt) keeps deceleration frame-rate independent.
        // BRAKE_FACTOR = 0.06 → ~94% speed reduction per second of coasting.
        const float decay = std::pow(BRAKE_FACTOR, dt);
        body.velocity.x *= decay;
        body.velocity.y *= decay;

        // Clamp micro-velocities to zero to avoid infinite crawl
        if (std::abs(body.velocity.x) < 2.0f && std::abs(body.velocity.y) < 2.0f)
          body.velocity = {0.0f, 0.0f};
      }
    }
  }

 private:
  // Nitro speed multiplier — 2.5× makes the boost very visible
  static constexpr float NITRO_MULTIPLIER = 2.5f;
  // Exponential brake base: lower = more grip/friction, higher = longer slide
  // 0.06 → ~94% speed lost per second of no throttle
  static constexpr float BRAKE_FACTOR = 0.06f;
};

#endif  // CAR_MOVEMENT_SYSTEM_HPP
