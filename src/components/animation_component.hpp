#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include <SDL2/SDL.h>

struct AnimationComponent {
  int numberOfFrames;
  int currentFrame;
  int frameSpeedRate;  // Time in milliseconds between frame changes
  bool isLoop;
  int startTime;

  AnimationComponent(int numberOfFrames = 1, int frameSpeedRate = 1,
                     bool isLoop = true)
      : numberOfFrames(numberOfFrames),
        currentFrame(1),
        frameSpeedRate(frameSpeedRate),
        isLoop(isLoop),
        startTime(SDL_GetTicks()) {}
};

#endif  // ANIMATION_COMPONENT_HPP