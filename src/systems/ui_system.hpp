#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>
#include <string>

#include "../ECS/ECS.hpp"
#include "../components/clickable_component.hpp"
#include "../components/script_component.hpp"
#include "../components/text_component.hpp"
#include "../components/transform_component.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/click_event.hpp"

class UISystem : public System {
 public:
  UISystem() {
    requireComponent<ClickableComponent>();
    requireComponent<TextComponent>();
    requireComponent<TransformComponent>();
  }

  void subscribeToClickEvent(std::unique_ptr<EventManager>& eventManager) {
    eventManager->subscribeToEvent<ClickEvent, UISystem>(
        this, &UISystem::onClickEvent);
  }

  void onClickEvent(ClickEvent& clickEvent) {
    for (Entity entity : getEntities()) {
      const auto& text = entity.getComponent<TextComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();
      if (transform.position.x < clickEvent.posX &&
          clickEvent.posX < transform.position.x + text.width &&
          transform.position.y < clickEvent.posY &&
          clickEvent.posY < transform.position.y + text.height) {
        if (entity.hasComponent<ScriptComponent>()) {
          const auto& script = entity.getComponent<ScriptComponent>();
          if (script.onClick != sol::lua_nil) {
            script.onClick();
          }
        }
      }
    }
  }
};
#endif  // UI_SYSTEM_HPP