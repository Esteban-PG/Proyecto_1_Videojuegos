#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include <SDL2/SDL.h>

#include <string>

struct TextComponent {
  std::string text;
  std::string fontId;
  SDL_Color   color;
  int         width;
  int         height;
  /**
   * When true the text is rendered in screen space (no camera offset applied).
   * Use this for HUD elements that should stay fixed on screen.
   */
  bool        isFixed;

  TextComponent(const std::string& text = "", const std::string& fontId = "",
                SDL_Color color = {0, 0, 0, 0}, bool isFixed = false)
      : text(text), fontId(fontId), color(color),
        width(0), height(0), isFixed(isFixed) {}
};

#endif  // TEXT_COMPONENT_HPP