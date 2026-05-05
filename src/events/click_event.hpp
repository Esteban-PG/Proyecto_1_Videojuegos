#ifndef CLICK_EVENT_HPP
#define CLICK_EVENT_HPP

#include "../ECS/ECS.hpp"
#include "../event_manager/event.hpp"

class ClickEvent : public Event {
 public:
  int buttonCode;
  int posX;
  int posY;

  ClickEvent(int buttonCode = 0, int posX = 0, int posY = 0)
      : buttonCode(buttonCode), posX(posX), posY(posY) {}
};

#endif  // CLICK_EVENT_HPP