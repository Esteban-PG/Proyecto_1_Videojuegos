#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <SDL2/SDL.h>

#include "../ECS/ECS.hpp"
#include "../asset_manager/asset_manager.hpp"
#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"

/**
 * @brief Renders all entities that have a SpriteComponent and a TransformComponent.
 *
 * World-space entities are drawn offset by the camera (cameraX/cameraY).
 * Entities with SpriteComponent::isFixed = true are drawn in screen space
 * (position is NOT offset by camera) — useful for HUD panels.
 *
 * SDL_RenderCopyEx is used so that TransformComponent::rotation is applied
 * around the centre of the destination rectangle, giving correct car rotation.
 */
class RenderSystem : public System {
 public:
  RenderSystem() {
    std::cout << "[RENDER SYSTEM] Constructor called" << std::endl;
    requireComponent<SpriteComponent>();
    requireComponent<TransformComponent>();
  }

  /**
   * @brief Draw all sprite entities.
   * @param renderer     SDL renderer
   * @param assetManager Texture source
   * @param cameraX      World-to-screen X offset (camera left-edge in world space)
   * @param cameraY      World-to-screen Y offset (camera top-edge in world space)
   */
  void update(SDL_Renderer* renderer,
              const std::unique_ptr<AssetManager>& assetManager,
              float cameraX = 0.0f, float cameraY = 0.0f) {
    for (Entity entity : getEntities()) {
      const auto& sprite    = entity.getComponent<SpriteComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();

      SDL_Texture* tex = assetManager->getTexture(sprite.textureId);
      if (!tex) continue;

      SDL_Rect srcRect = sprite.srcRect;

      // Apply camera offset for world-space entities only
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

      SDL_RenderCopyEx(renderer, tex, &srcRect, &dstRect,
                       transform.rotation, nullptr, SDL_FLIP_NONE);
    }
  }
};

#endif  // RENDER_SYSTEM_HPP
