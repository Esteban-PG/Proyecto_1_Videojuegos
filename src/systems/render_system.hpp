#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <SDL2/SDL.h>

#include "../ECS/ECS.hpp"
#include "../asset_manager/asset_manager.hpp"
#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"

class RenderSystem : public System {
 public:
  RenderSystem() {
    std::cout << "[RENDER SYSTEM] Constructor called" << std::endl;
    requireComponent<SpriteComponent>();
    requireComponent<TransformComponent>();
  }

  void update(SDL_Renderer* renderer,
              const std::unique_ptr<AssetManager>& assetManager,
              float cameraX = 0.0f, float cameraY = 0.0f) {
    for (Entity entity : getEntities()) {
      const auto& sprite    = entity.getComponent<SpriteComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();

      SDL_Texture* tex = assetManager->getTexture(sprite.textureId);
      if (!tex) continue;

      SDL_Rect srcRect = sprite.srcRect;

      const float drawX = sprite.isFixed
                        ? transform.position.x
                        : transform.position.x - cameraX;
      const float drawY = sprite.isFixed
                        ? transform.position.y
                        : transform.position.y - cameraY;

      SDL_Rect dstRect = {
          static_cast<int>(drawX),
          static_cast<int>(drawY),
          static_cast<int>(sprite.width  * transform.scale.x),
          static_cast<int>(sprite.height * transform.scale.y)
      };

      SDL_RendererFlip flip = sprite.flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
      SDL_RenderCopyEx(renderer, tex, &srcRect, &dstRect,
                       transform.rotation, nullptr, flip);
    }
  }
};

#endif  // RENDER_SYSTEM_HPP
