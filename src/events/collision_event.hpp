#ifndef COLLISION_EVENT_HPP
#define COLLISION_EVENT_HPP

#include "../ECS/ECS.hpp"
#include "../event_manager/event.hpp"

class CollisionEvent : public Event {
 public:
  Entity entityA;
  Entity entityB;

  CollisionEvent(Entity entityA, Entity entityB)
      : entityA(entityA), entityB(entityB) {}
};

#endif  // COLLISION_EVENT_HPP