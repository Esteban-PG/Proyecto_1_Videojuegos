#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include <SDL2/SDL.h>

struct AnimationComponent {
  int  numberOfFrames; ///< Frames to play in a single cycle (within one row)
  int  framesPerRow;   ///< Frames per row in the sheet
  int  baseRow;        ///< Which row of the sheet to use (0 = top row)
  int  currentFrame;
  int  frameSpeedRate; ///< Frames per second
  bool isLoop;
  int  startTime;

  AnimationComponent(int numberOfFrames = 1,
                     int frameSpeedRate  = 1,
                     bool isLoop         = true,
                     int framesPerRow    = 0,
                     int baseRow         = 0)
      : numberOfFrames(numberOfFrames),
        framesPerRow(framesPerRow > 0 ? framesPerRow : numberOfFrames),
        baseRow(baseRow),
        currentFrame(0),
        frameSpeedRate(frameSpeedRate),
        isLoop(isLoop),
        startTime(SDL_GetTicks()) {}
};

#endif  // ANIMATION_COMPONENT_HPP
