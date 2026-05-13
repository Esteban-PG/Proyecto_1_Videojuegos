#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include <glm/glm.hpp>

/**
 * @brief Stores the current velocity of an entity (pixels per second).
 *
 * MovementSystem integrates velocity into TransformComponent::position
 * every frame: position += velocity * deltaTime.
 */
struct RigidBodyComponent {
  glm::vec2 velocity; ///< Current velocity in world pixels per second (x, y)

  RigidBodyComponent(const glm::vec2& velocity = glm::vec2(0.0f))
      : velocity(velocity) {}
};

#endif  // RIGID_BODY_COMPONENT_HPP