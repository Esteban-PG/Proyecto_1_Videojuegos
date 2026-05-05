#ifndef SCENE_LOADER_HPP
#define SCENE_LOADER_HPP

#include <SDL2/SDL.h>

#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "../ECS/ECS.hpp"
#include "../asset_manager/asset_manager.hpp"
#include "../controller_manager/controller_manager.hpp"

/**
 * @brief Parses and instantiates a Lua scene definition.
 *
 * A scene file is a Lua script that sets a global `scene` table with the
 * following optional sections (all sections are optional):
 *
 *   scene.color_textures  — solid-colour placeholder textures
 *   scene.sprites         — file-based textures (PNG / BMP)
 *   scene.fonts           — TTF fonts
 *   scene.keys            — keyboard action bindings
 *   scene.buttons         — mouse button bindings
 *   scene.entities        — entity definitions with components
 *
 * Component types supported inside entity definitions:
 *   animation, box_collider, circle_collider, clickable, nitro,
 *   player, rigid_body, script, sprite, tag, text, transform
 */
class SceneLoader {
 private:
  void loadColorTextures(SDL_Renderer* renderer,
                         const sol::table& colorTextures,
                         std::unique_ptr<AssetManager>& assetManager);
  void loadSprites(SDL_Renderer* renderer, const sol::table& sprites,
                   std::unique_ptr<AssetManager>& assetManager);
  void loadFonts(const sol::table& fonts,
                 std::unique_ptr<AssetManager>& assetManager);
  void loadKeys(const sol::table& keys,
                std::unique_ptr<ControllerManager>& controllerManager);
  void loadButtons(const sol::table& buttons,
                   std::unique_ptr<ControllerManager>& controllerManager);
  void loadEntities(sol::state& lua, const sol::table& entities,
                    std::unique_ptr<Registry>& registry);

 public:
  SceneLoader();
  ~SceneLoader();

  void loadScene(const std::string& sceneFilePath, sol::state& lua,
                 std::unique_ptr<AssetManager>& assetManager,
                 std::unique_ptr<ControllerManager>& controllerManager,
                 std::unique_ptr<Registry>& registry, SDL_Renderer* renderer);
};

#endif  // SCENE_LOADER_HPP
