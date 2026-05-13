#ifndef CLICK_EVENT_HPP
#define CLICK_EVENT_HPP

#include "../ECS/ECS.hpp"
#include "../event_manager/event.hpp"

/**
 * @brief Fired when the player clicks a mouse button.
 *
 * Emitted by Game::processInput() on SDL_MOUSEBUTTONDOWN.
 * UISystem subscribes to this event to detect button presses.
 */
class ClickEvent : public Event {
 public:
  int buttonCode; ///< SDL mouse button code (1 = left, 3 = right)
  int posX;       ///< Screen X position of the click in pixels
  int posY;       ///< Screen Y position of the click in pixels

  ClickEvent(int buttonCode = 0, int posX = 0, int posY = 0)
      : buttonCode(buttonCode), posX(posX), posY(posY) {}
};

#endif  // CLICK_EVENT_HPP