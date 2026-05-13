#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include <SDL2/SDL.h>

/**
 * @brief Drives sprite-sheet frame animation for an entity.
 *
 * AnimationSystem advances currentFrame each tick based on frameSpeedRate
 * and writes the result into SpriteComponent::srcRect.x.
 */
struct AnimationComponent {
  int  numberOfFrames; ///< Total frames in the animation strip
  int  currentFrame;   ///< Currently displayed frame index (0-based)
  int  frameSpeedRate; ///< Frames per second
  bool isLoop;         ///< Whether the animation wraps around at the last frame
  int  startTime;      ///< SDL_GetTicks() timestamp when animation began

  AnimationComponent(int numberOfFrames = 1, int frameSpeedRate = 1,
                     bool isLoop = true)
      : numberOfFrames(numberOfFrames),
        currentFrame(1),
        frameSpeedRate(frameSpeedRate),
        isLoop(isLoop),
        startTime(SDL_GetTicks()) {}
};

#endif  // ANIMATION_COMPONENT_HPP