#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

/**
 * @brief Base class for all game events.
 *
 * Concrete event types (ClickEvent, CollisionEvent) inherit from this class.
 * EventManager uses type-erasure via IEventCallback to dispatch events to
 * the correct subscriber without requiring a common interface beyond this base.
 */
class Event {
 public:
  Event() = default;
};

#endif  // EVENT_HPP