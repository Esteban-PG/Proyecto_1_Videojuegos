#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/transform_component.hpp"

class MovementSystem : public System {
 public:
  MovementSystem() {
    std::cout << "[MOVEMENT SYSTEM] Constructor called" << std::endl;
    requireComponent<TransformComponent>();
    requireComponent<RigidBodyComponent>();
  }

  void update(float deltaTime) {
    for (Entity entity : getEntities()) {
      auto& transform = entity.getComponent<TransformComponent>();
      const auto& rigidBody = entity.getComponent<RigidBodyComponent>();

      transform.position += rigidBody.velocity * deltaTime;
    }
  }
};

#endif  // MOVEMENT_SYSTEM_HPP