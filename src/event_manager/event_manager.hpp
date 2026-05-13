#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "event.hpp"

/**
 * @brief Type-erased interface for a single event subscription.
 *
 * Concrete instantiations are created by EventCallback<TOwner,TEvent>.
 * EventManager stores a list of these per event type.
 */
class IEventCallback {
 private:
  virtual void call(Event& e) = 0;

 public:
  virtual ~IEventCallback() = default;
  /** @brief Dispatch the event to the concrete callback. */
  void execute(Event& e) { call(e); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
 private:
  typedef void (TOwner::*CallbackFunction)(TEvent&);
  TOwner* ownerInstance;
  CallbackFunction callbackFunction;

  virtual void call(Event& e) override {
    std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
  }

 public:
  EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
      : ownerInstance(ownerInstance), callbackFunction(callbackFunction) {}
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

/**
 * @brief Publish-subscribe event bus for decoupled system communication.
 *
 * Systems subscribe to specific event types at the start of each frame
 * (subscriptions are cleared on reset() so stale handlers never fire).
 * Events are emitted synchronously — all handlers run before emitEvent returns.
 *
 * Usage:
 * @code
 *   // Subscribe (called each frame before update):
 *   eventManager->subscribeToEvent<CollisionEvent>(this, &MySystem::onCollision);
 *   // Emit:
 *   eventManager->emitEvent<CollisionEvent>(entityA, entityB);
 * @endcode
 */
class EventManager {
 private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

 public:
  EventManager() {
    std::cout << "[EVENT MANAGER] Constructor called" << std::endl;
  }

  ~EventManager() {
    std::cout << "[EVENT MANAGER] Destructor called" << std::endl;
  }

  /** @brief Clear all subscriptions (call at the start of each frame). */
  void reset() { subscribers.clear(); }

  /**
   * @brief Subscribe a member function to an event type.
   * @tparam TEvent    Event type to listen for
   * @tparam TOwner    Class that owns the callback method
   * @param ownerInstance   Pointer to the listener object
   * @param callbackFunction Member function pointer called when the event fires
   */
  template <typename TEvent, typename TOwner>
  void subscribeToEvent(TOwner* ownerInstance,
                        void (TOwner::*callbackFunction)(TEvent&)) {
    if (!subscribers[typeid(TEvent)].get()) {
      subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(
        ownerInstance, callbackFunction);
    subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
  }

  /**
   * @brief Construct and dispatch an event to all current subscribers.
   * @tparam TEvent  Event type to emit
   * @tparam TArgs   Constructor argument types forwarded to TEvent
   * @param args     Arguments forwarded to the TEvent constructor
   */
  template <typename TEvent, typename... TArgs>
  void emitEvent(TArgs&&... args) {
    auto handlers = subscribers[typeid(TEvent)].get();
    if (handlers) {
      for (auto it = handlers->begin(); it != handlers->end(); ++it) {
        auto handler = it->get();
        TEvent event(std::forward<TArgs>(args)...);
        handler->execute(event);
      }
    }
  }
};

#endif  // EVENT_MANAGER_HPP