#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <iostream>
#include <memory>

#include "../ECS/ECS.hpp"
#include "../components/circle_collider_component.hpp"
#include "../components/transform_component.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/collision_event.hpp"

/**
 * @brief Detects circle-circle overlaps and emits CollisionEvents.
 *
 * Operates on all entities with CircleColliderComponent + TransformComponent.
 * Runs an O(n²) sweep each frame and emits a CollisionEvent for every
 * overlapping pair. DamageSystem processes those events to apply gameplay effects.
 *
 * Center convention: center = position - (width/2 * scale, height/2 * scale).
 */
class CollisionSystem : public System {
 public:
  CollisionSystem() {
    requireComponent<CircleColliderComponent>();
    requireComponent<TransformComponent>();
  }

  /**
   * @brief Run one collision pass and emit events for all overlapping pairs.
   * @param eventManager Event bus used to emit CollisionEvent instances
   */
  void update(std::unique_ptr<EventManager>& eventManager) {
    auto entities = getEntities();
    for (auto i = entities.begin(); i != entities.end(); ++i) {
      Entity entityA = *i;
      const auto& colliderA = entityA.getComponent<CircleColliderComponent>();
      const auto& transformA = entityA.getComponent<TransformComponent>();
      for (auto j = i; j != entities.end(); ++j) {
        Entity entityB = *j;
        if (entityA == entityB) continue;

        const auto& colliderB = entityB.getComponent<CircleColliderComponent>();
        const auto& transformB = entityB.getComponent<TransformComponent>();

        glm::vec2 aCenterPosition =
            glm::vec2(transformA.position.x -
                          (colliderA.width / 2.0f) * transformA.scale.x,
                      transformA.position.y -
                          (colliderA.height / 2.0f) * transformA.scale.y);
        glm::vec2 bCenterPosition =
            glm::vec2(transformB.position.x -
                          (colliderB.width / 2.0f) * transformB.scale.x,
                      transformB.position.y -
                          (colliderB.height / 2.0f) * transformB.scale.y);

        int aRadius = static_cast<int>(colliderA.radius * transformA.scale.x);
        int bRadius = static_cast<int>(colliderB.radius * transformB.scale.x);

        bool collision = checkCircularCollision(
            aRadius, bRadius, aCenterPosition, bCenterPosition);

        if (collision) {
          // Handle collision event
          eventManager->emitEvent<CollisionEvent>(entityA, entityB);
        }
      }
    }
  }
  /**
   * @brief @return True if two circles with the given radii and centres overlap.
   * @param aRadius Scaled radius of entity A
   * @param bRadius Scaled radius of entity B
   * @param aCenterPosition World-space centre of entity A
   * @param bCenterPosition World-space centre of entity B
   */
  bool checkCircularCollision(int aRadius, int bRadius,
                              glm::vec2 aCenterPosition,
                              glm::vec2 bCenterPosition) {
    glm::vec2 dif = aCenterPosition - bCenterPosition;
    float length = glm::sqrt((dif.x * dif.x) + (dif.y * dif.y));
    float radiusSum = aRadius + bRadius;
    return length < radiusSum;
  }
};
#endif  // COLLISION_SYSTEM_HPP