#include "scene_loader.hpp"

#include <glm/glm.hpp>
#include <iostream>

#include "../components/animation_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/circle_collider_component.hpp"
#include "../components/clickable_component.hpp"
#include "../components/nitro_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/script_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/tag_component.hpp"
#include "../components/text_component.hpp"
#include "../components/transform_component.hpp"

// ─────────────────────────────────────────────────────────────────────────────

SceneLoader::SceneLoader() {
  std::cout << "[SCENE LOADER] Constructor called." << std::endl;
}

SceneLoader::~SceneLoader() {
  std::cout << "[SCENE LOADER] Destructor called." << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// loadScene
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadScene(
    const std::string& sceneFilePath, sol::state& lua,
    std::unique_ptr<AssetManager>& assetManager,
    std::unique_ptr<ControllerManager>& controllerManager,
    std::unique_ptr<Registry>& registry, SDL_Renderer* renderer) {

  sol::load_result script = lua.load_file(sceneFilePath);
  if (!script.valid()) {
    sol::error err = script;
    std::cerr << "[SCENE LOADER] Failed to load scene: " << err.what() << std::endl;
    return;
  }
  lua.script_file(sceneFilePath);
  sol::table scene = lua["scene"];

  // ── Assets ────────────────────────────────────────────────────────────────
  sol::optional<sol::table> hasColorTex = scene["color_textures"];
  if (hasColorTex != sol::nullopt)
    loadColorTextures(renderer, scene["color_textures"], assetManager);

  sol::optional<sol::table> hasSprites = scene["sprites"];
  if (hasSprites != sol::nullopt)
    loadSprites(renderer, scene["sprites"], assetManager);

  sol::optional<sol::table> hasFonts = scene["fonts"];
  if (hasFonts != sol::nullopt)
    loadFonts(scene["fonts"], assetManager);

  // ── Input bindings ────────────────────────────────────────────────────────
  sol::optional<sol::table> hasKeys = scene["keys"];
  if (hasKeys != sol::nullopt)
    loadKeys(scene["keys"], controllerManager);

  sol::optional<sol::table> hasButtons = scene["buttons"];
  if (hasButtons != sol::nullopt)
    loadButtons(scene["buttons"], controllerManager);

  // ── Entities ──────────────────────────────────────────────────────────────
  sol::optional<sol::table> hasEntities = scene["entities"];
  if (hasEntities != sol::nullopt)
    loadEntities(lua, scene["entities"], registry);
}

// ─────────────────────────────────────────────────────────────────────────────
// loadColorTextures
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadColorTextures(SDL_Renderer* renderer,
                                    const sol::table& colorTextures,
                                    std::unique_ptr<AssetManager>& assetManager) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> entry = colorTextures[i];
    if (!entry) break;
    sol::table t = colorTextures[i];
    std::string id = t["assetId"];
    Uint8 r = t["r"].get_or<Uint8>(255);
    Uint8 g = t["g"].get_or<Uint8>(255);
    Uint8 b = t["b"].get_or<Uint8>(255);
    Uint8 a = t["a"].get_or<Uint8>(255);
    assetManager->addColorTexture(renderer, id, r, g, b, a);
    ++i;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// loadSprites
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadSprites(SDL_Renderer* renderer, const sol::table& sprites,
                              std::unique_ptr<AssetManager>& assetManager) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> entry = sprites[i];
    if (!entry) break;
    sol::table sprite = sprites[i];
    std::string assetId  = sprite["assetId"];
    std::string filePath = sprite["filePath"];
    assetManager->addTexture(renderer, assetId, filePath);
    ++i;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// loadFonts
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadFonts(const sol::table& fonts,
                            std::unique_ptr<AssetManager>& assetManager) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> entry = fonts[i];
    if (!entry) break;
    sol::table font = fonts[i];
    std::string fontId   = font["assetId"];
    std::string filePath = font["filePath"];
    int         fontSize = font["fontSize"];
    assetManager->addFont(fontId, filePath, fontSize);
    ++i;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// loadKeys / loadButtons
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadKeys(const sol::table& keys,
                           std::unique_ptr<ControllerManager>& controllerManager) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> entry = keys[i];
    if (!entry) break;
    sol::table key = keys[i];
    std::string action = key["action"];
    int         code   = key["key"];
    controllerManager->addActionKey(action, code);
    ++i;
  }
}

void SceneLoader::loadButtons(const sol::table& buttons,
                              std::unique_ptr<ControllerManager>& controllerManager) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> entry = buttons[i];
    if (!entry) break;
    sol::table btn  = buttons[i];
    std::string name = btn["name"];
    int         code = btn["button"];
    controllerManager->addMouseButton(name, code);
    ++i;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// loadEntities
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadEntities(sol::state& lua, const sol::table& entities,
                               std::unique_ptr<Registry>& registry) {
  int i = 0;
  while (true) {
    sol::optional<sol::table> hasEntity = entities[i];
    if (!hasEntity) break;

    sol::table entityDef = entities[i];
    Entity newEntity = registry->createEntity();

    sol::optional<sol::table> hasComponents = entityDef["components"];
    if (hasComponents == sol::nullopt) { ++i; continue; }

    sol::table c = entityDef["components"];

    // ── AnimationComponent ────────────────────────────────────────────────
    if (c["animation"] != sol::lua_nil) {
      sol::table anim = c["animation"];
      newEntity.addComponent<AnimationComponent>(
          anim["num_frames"].get_or(1),
          anim["frame_rate_speed"].get_or(1),
          anim["is_looping"].get_or(true));
    }

    // ── BoxColliderComponent ──────────────────────────────────────────────
    if (c["box_collider"] != sol::lua_nil) {
      sol::table bc = c["box_collider"];
      newEntity.addComponent<BoxColliderComponent>(
          bc["width"].get_or(32.0f),
          bc["height"].get_or(32.0f));
    }

    // ── CircleColliderComponent ───────────────────────────────────────────
    if (c["circle_collider"] != sol::lua_nil) {
      sol::table cc = c["circle_collider"];
      newEntity.addComponent<CircleColliderComponent>(
          cc["radius"].get_or(16.0f),
          cc["width"].get_or(32),
          cc["height"].get_or(32));
    }

    // ── ClickableComponent ────────────────────────────────────────────────
    if (c["clickable"] != sol::lua_nil) {
      newEntity.addComponent<ClickableComponent>();
    }

    // ── NitroComponent ────────────────────────────────────────────────────
    if (c["nitro"] != sol::lua_nil) {
      sol::table n = c["nitro"];
      newEntity.addComponent<NitroComponent>(
          n["cooldown"].get_or(2.0f),
          n["boost_duration"].get_or(0.5f));
    }

    // ── PlayerComponent ───────────────────────────────────────────────────
    if (c["player"] != sol::lua_nil) {
      sol::table p = c["player"];
      newEntity.addComponent<PlayerComponent>(
          p["base_speed"].get_or(290.0f),
          p["rotation_speed"].get_or(130.0f),
          p["jump_duration"].get_or(2.5f),
          p["jump_max_scale"].get_or(1.7f),
          p["base_scale_x"].get_or(1.0f),
          p["base_scale_y"].get_or(1.0f));
    }

    // ── RigidBodyComponent ────────────────────────────────────────────────
    if (c["rigid_body"] != sol::lua_nil) {
      sol::table rb = c["rigid_body"];
      newEntity.addComponent<RigidBodyComponent>(
          glm::vec2(rb["velocity"]["x"].get_or(0.0f),
                    rb["velocity"]["y"].get_or(0.0f)));
    }

    // ── ScriptComponent ───────────────────────────────────────────────────
    if (c["script"] != sol::lua_nil) {
      lua["on_click"] = sol::lua_nil;
      lua["update"]   = sol::lua_nil;
      std::string path = c["script"]["path"];
      lua.script_file(path);

      sol::function onClick = sol::lua_nil;
      sol::optional<sol::function> hasClick = lua["on_click"];
      if (hasClick) onClick = lua["on_click"];

      sol::function updateFn = sol::lua_nil;
      sol::optional<sol::function> hasUpdate = lua["update"];
      if (hasUpdate) updateFn = lua["update"];

      newEntity.addComponent<ScriptComponent>(updateFn, onClick);
    }

    // ── SpriteComponent ───────────────────────────────────────────────────
    if (c["sprite"] != sol::lua_nil) {
      sol::table s = c["sprite"];
      bool isFixed = s["is_fixed"].get_or(false);
      newEntity.addComponent<SpriteComponent>(
          s["asset_id"].get_or<std::string>("none"),
          s["width"].get_or(32),
          s["height"].get_or(32),
          s["src_rect"]["x"].get_or(0),
          s["src_rect"]["y"].get_or(0),
          isFixed);
    }

    // ── TagComponent ──────────────────────────────────────────────────────
    if (c["tag"] != sol::lua_nil) {
      std::string tag = c["tag"]["tag"].get_or<std::string>("");
      newEntity.addComponent<TagComponent>(tag);
    }

    // ── TextComponent ─────────────────────────────────────────────────────
    if (c["text"] != sol::lua_nil) {
      sol::table t = c["text"];
      bool isFixed = t["is_fixed"].get_or(false);
      newEntity.addComponent<TextComponent>(
          t["text"].get_or<std::string>(""),
          t["font_asset_id"].get_or<std::string>(""),
          SDL_Color{
              static_cast<Uint8>(t["color"]["r"].get_or(255)),
              static_cast<Uint8>(t["color"]["g"].get_or(255)),
              static_cast<Uint8>(t["color"]["b"].get_or(255)),
              static_cast<Uint8>(t["color"]["a"].get_or(255))
          },
          isFixed);
    }

    // ── TransformComponent ────────────────────────────────────────────────
    if (c["transform"] != sol::lua_nil) {
      sol::table t = c["transform"];
      newEntity.addComponent<TransformComponent>(
          glm::vec2(t["position"]["x"].get_or(0.0f),
                    t["position"]["y"].get_or(0.0f)),
          glm::vec2(t["scale"]["x"].get_or(1.0f),
                    t["scale"]["y"].get_or(1.0f)),
          t["rotation"].get_or(0.0));
    }

    ++i;
  }
}
