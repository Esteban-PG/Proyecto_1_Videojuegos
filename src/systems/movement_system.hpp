#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/transform_component.hpp"

/**
 * @brief Integrates RigidBodyComponent velocity into TransformComponent position.
 *
 * Applies position += velocity * deltaTime each frame for every entity
 * that has both a TransformComponent and a RigidBodyComponent.
 * Car-specific velocity is set by CarMovementSystem before this runs.
 */
class MovementSystem : public System {
 public:
  MovementSystem() {
    std::cout << "[MOVEMENT SYSTEM] Constructor called" << std::endl;
    requireComponent<TransformComponent>();
    requireComponent<RigidBodyComponent>();
  }

  /**
   * @brief Integrate velocity into position for all moving entities.
   * @param deltaTime Seconds elapsed since the last frame
   */
  void update(float deltaTime) {
    for (Entity entity : getEntities()) {
      auto& transform = entity.getComponent<TransformComponent>();
      const auto& rigidBody = entity.getComponent<RigidBodyComponent>();

      transform.position += rigidBody.velocity * deltaTime;
    }
  }
};

#endif  // MOVEMENT_SYSTEM_HPP