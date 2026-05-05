#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <SDL2/SDL.h>

#include <string>

class SpriteComponent {
 public:
  std::string textureId;
  int      width;
  int      height;
  SDL_Rect srcRect;
  /**
   * When true the sprite is rendered in screen space (no camera offset).
   * Useful for HUD panels or overlays.
   */
  bool     isFixed;

  SpriteComponent(const std::string& textureId = "none", int width = 0,
                  int height = 0, int srcRectX = 0, int srcRectY = 0,
                  bool isFixed = false) {
    this->textureId = textureId;
    this->width     = width;
    this->height    = height;
    this->srcRect   = {srcRectX, srcRectY, width, height};
    this->isFixed   = isFixed;
  }
};

#endif  // SPRITE_COMPONENT_HPP