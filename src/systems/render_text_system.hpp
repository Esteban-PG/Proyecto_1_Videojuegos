#ifndef RENDER_TEXT_SYSTEM_HPP
#define RENDER_TEXT_SYSTEM_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include "../ECS/ECS.hpp"
#include "../asset_manager/asset_manager.hpp"
#include "../components/text_component.hpp"
#include "../components/transform_component.hpp"

/**
 * @brief Renders entities that have TextComponent + TransformComponent.
 *
 * Camera offset is applied to world-space text (isFixed = false).
 * HUD text entities should set TextComponent::isFixed = true so they
 * always appear at the same screen position regardless of camera movement.
 *
 * Text textures are created and destroyed every frame (no caching) — this is
 * sufficient for games with a small number of text entities.
 */
class RenderTextSystem : public System {
 public:
  RenderTextSystem() {
    requireComponent<TextComponent>();
    requireComponent<TransformComponent>();
  }

  /**
   * @brief Render all text entities.
   * @param renderer     SDL renderer
   * @param assetManager Font source
   * @param cameraX      Camera left-edge in world space
   * @param cameraY      Camera top-edge in world space
   */
  void update(SDL_Renderer* renderer,
              const std::unique_ptr<AssetManager>& assetManager,
              float cameraX = 0.0f, float cameraY = 0.0f) {
    for (Entity entity : getEntities()) {
      auto& textComp  = entity.getComponent<TextComponent>();
      const auto& tf  = entity.getComponent<TransformComponent>();

      TTF_Font* font = assetManager->getFont(textComp.fontId);
      if (!font) continue;

      SDL_Surface* surface = TTF_RenderText_Blended(
          font, textComp.text.c_str(), textComp.color);
      if (!surface) continue;

      textComp.width  = surface->w;
      textComp.height = surface->h;
      SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
      if (!tex) continue;

      const float drawX = textComp.isFixed
                        ? tf.position.x
                        : tf.position.x - cameraX;
      const float drawY = textComp.isFixed
                        ? tf.position.y
                        : tf.position.y - cameraY;

      SDL_Rect destRect = {
          static_cast<int>(drawX),
          static_cast<int>(drawY),
          textComp.width,
          textComp.height
      };

      SDL_RenderCopy(renderer, tex, nullptr, &destRect);
      SDL_DestroyTexture(tex);
    }
  }
};

#endif  // RENDER_TEXT_SYSTEM_HPP
