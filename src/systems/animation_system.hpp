#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include <SDL2/SDL.h>

#include "../ECS/ECS.hpp"
#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"

/**
 * @brief Advances sprite-sheet frame animations each tick.
 *
 * Supports single-row strips and multi-row sheets via framesPerRow.
 * Non-looping animations (isLoop=false) freeze on the last frame.
 */
class AnimationSystem : public System {
 public:
  AnimationSystem() {
    requireComponent<AnimationComponent>();
    requireComponent<SpriteComponent>();
  }

  void update() {
    for (Entity entity : getEntities()) {
      auto& anim   = entity.getComponent<AnimationComponent>();
      auto& sprite = entity.getComponent<SpriteComponent>();

      int elapsed = static_cast<int>(SDL_GetTicks() - anim.startTime)
                    * anim.frameSpeedRate / 1000;

      int frame;
      if (anim.isLoop) {
        frame = elapsed % anim.numberOfFrames;
      } else {
        frame = std::min(elapsed, anim.numberOfFrames - 1);
      }
      anim.currentFrame = frame;

      int col = frame % anim.framesPerRow;
      sprite.srcRect.x = sprite.width  * col;
      sprite.srcRect.y = sprite.height * anim.baseRow;
    }
  }
};

#endif  // ANIMATION_SYSTEM_HPP
