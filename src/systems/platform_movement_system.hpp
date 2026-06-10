#ifndef PLATFORM_MOVEMENT_SYSTEM_HPP
#define PLATFORM_MOVEMENT_SYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"
#include "../systems/tag_system.hpp"

class PlatformMovementSystem : public System {
 public:
  PlatformMovementSystem() {
    requireComponent<PlayerComponent>();
    requireComponent<RigidBodyComponent>();
    requireComponent<TransformComponent>();
  }

  void update(double dt) {
    auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
    Entity player = tagSys.getEntityByTag("player");
    if (player.getId() < 0) return;
    if (!player.hasComponent<PlayerComponent>()) return;

    auto& pc = player.getComponent<PlayerComponent>();
    auto& rb = player.getComponent<RigidBodyComponent>();

    if (pc.isDead || pc.hasWon) {
      rb.velocity = {0.0f, 0.0f};
      return;
    }

    // Reset isOnGround each frame; BoxCollisionSystem sets it back if on floor
    pc.isOnGround = false;

    // 1. Apply gravity (capped at terminal velocity)
    pc.velocityY += pc.gravity * static_cast<float>(dt);
    if (pc.velocityY > 800.0f) pc.velocityY = 800.0f;

    // 2. Horizontal movement
    auto& ctrl = *Game::getInstance().controllerManager;
    if (ctrl.isActionActivated("move_left")) {
      rb.velocity.x = -pc.baseSpeed;
    } else if (ctrl.isActionActivated("move_right")) {
      rb.velocity.x = pc.baseSpeed;
    } else {
      rb.velocity.x = 0.0f;
    }

    // 3. Jump
    if (ctrl.isActionActivated("jump") && pc.canJump) {
      pc.velocityY  = pc.jumpForce;
      pc.isOnGround = false;
      pc.canJump    = false;
    }

    // 4. Push vertical velocity into RigidBody for MovementSystem to integrate
    rb.velocity.y = pc.velocityY;
  }
};

#endif  // PLATFORM_MOVEMENT_SYSTEM_HPP
