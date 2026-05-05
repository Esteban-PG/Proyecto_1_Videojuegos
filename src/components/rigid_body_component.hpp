#ifndef RIGID_BODY_COMPONENT_HPP
#define RIGID_BODY_COMPONENT_HPP

#include <glm/glm.hpp>

struct RigidBodyComponent {
  glm::vec2 velocity;

  RigidBodyComponent(const glm::vec2& velocity = glm::vec2(0.0f))
      : velocity(velocity) {}
};

#endif  // RIGID_BODY_COMPONENT_HPP