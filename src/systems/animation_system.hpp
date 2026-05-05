#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include <SDL2/SDL.h>

#include "../ECS/ECS.hpp"
#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"

class AnimationSystem : public System {
 public:
  AnimationSystem() {
    requireComponent<AnimationComponent>();
    requireComponent<SpriteComponent>();
  }

  void update() {
    for (Entity entity : getEntities()) {
      auto& animation = entity.getComponent<AnimationComponent>();
      auto& sprite = entity.getComponent<SpriteComponent>();

      animation.currentFrame = ((SDL_GetTicks() - animation.startTime) *
                                animation.frameSpeedRate / 1000) %
                               animation.numberOfFrames;
      sprite.srcRect.x = sprite.width * animation.currentFrame;
    }
  }
};

#endif  // ANIMATION_SYSTEM_HPP