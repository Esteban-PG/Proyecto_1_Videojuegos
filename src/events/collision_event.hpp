#ifndef COLLISION_EVENT_HPP
#define COLLISION_EVENT_HPP

#include "../ECS/ECS.hpp"
#include "../event_manager/event.hpp"

/**
 * @brief Fired when two circle-collider entities overlap.
 *
 * Emitted by CollisionSystem each frame for every overlapping pair.
 * DamageSystem subscribes to apply gameplay effects (e.g. player + saw → death).
 */
class CollisionEvent : public Event {
 public:
  Entity entityA; ///< First entity of the colliding pair
  Entity entityB; ///< Second entity of the colliding pair

  CollisionEvent(Entity entityA, Entity entityB)
      : entityA(entityA), entityB(entityB) {}
};

#endif  // COLLISION_EVENT_HPP