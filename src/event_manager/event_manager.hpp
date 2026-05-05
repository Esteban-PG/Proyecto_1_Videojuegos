#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "event.hpp"

class IEventCallback {
 private:
  virtual void call(Event& e) = 0;

 public:
  virtual ~IEventCallback() = default;
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

  void reset() { subscribers.clear(); }

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