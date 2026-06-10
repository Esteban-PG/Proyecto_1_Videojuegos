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
  bool     isFixed;
  bool     flipX;   ///< Mirror horizontally (e.g. player facing left)

  SpriteComponent(const std::string& textureId = "none", int width = 0,
                  int height = 0, int srcRectX = 0, int srcRectY = 0,
                  bool isFixed = false, bool flipX = false) {
    this->textureId = textureId;
    this->width     = width;
    this->height    = height;
    this->srcRect   = {srcRectX, srcRectY, width, height};
    this->isFixed   = isFixed;
    this->flipX     = flipX;
  }
};

#endif  // SPRITE_COMPONENT_HPP
