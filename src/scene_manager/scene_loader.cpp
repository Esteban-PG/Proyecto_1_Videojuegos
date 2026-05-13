#include "scene_loader.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

#include "../game/game.hpp"
#include "tinyxml2/tinyxml2.h"

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

  // ── Music ────────────────────────────────────────────────────────────────
  sol::optional<std::string> hasMusic = scene["music"];
  if (hasMusic != sol::nullopt)
    Game::getInstance().audioManager->playMusic(scene["music"].get<std::string>());

  // ── Map (TMX) ─────────────────────────────────────────────────────────────
  sol::optional<sol::table> hasMap = scene["map"];
  if (hasMap != sol::nullopt)
    loadMap(lua, scene["map"], registry, renderer, assetManager);

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

// ─────────────────────────────────────────────────────────────────────────────
// loadMap
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadMap(sol::state& lua, const sol::table& map,
                          std::unique_ptr<Registry>& registry,
                          SDL_Renderer* renderer,
                          std::unique_ptr<AssetManager>& assetManager) {
  sol::optional<std::string> hasPath = map["path"];
  if (hasPath == sol::nullopt) {
    std::cerr << "[SCENE LOADER] loadMap: missing 'path' in map table." << std::endl;
    return;
  }

  std::string path = map["path"];

  tinyxml2::XMLDocument xmlDoc;
  if (xmlDoc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << "[SCENE LOADER] loadMap: failed to load TMX file: " << path << std::endl;
    return;
  }

  tinyxml2::XMLElement* xmlRoot = xmlDoc.RootElement();

  int tWidth = 0, tHeight = 0, mWidth = 0, mHeight = 0;
  xmlRoot->QueryIntAttribute("tilewidth",  &tWidth);
  xmlRoot->QueryIntAttribute("tileheight", &tHeight);
  xmlRoot->QueryIntAttribute("width",      &mWidth);
  xmlRoot->QueryIntAttribute("height",     &mHeight);

  Game::getInstance().mapWidth  = tWidth  * mWidth;
  Game::getInstance().mapHeight = tHeight * mHeight;

  // ── Leer el tileset externo (.tsx) para obtener columnas e imagen ──────────
  int tilesetColumns = mWidth; // fallback si no se puede leer el TSX
  std::string tmxDir = path.substr(0, path.find_last_of("/\\") + 1);

  tinyxml2::XMLElement* tilesetEl = xmlRoot->FirstChildElement("tileset");
  if (tilesetEl) {
    const char* source = tilesetEl->Attribute("source");
    if (source) {
      std::string tsxPath = tmxDir + source;
      tinyxml2::XMLDocument tsxDoc;
      if (tsxDoc.LoadFile(tsxPath.c_str()) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement* tsxRoot = tsxDoc.RootElement();
        tsxRoot->QueryIntAttribute("columns", &tilesetColumns);

        tinyxml2::XMLElement* imgEl = tsxRoot->FirstChildElement("image");
        if (imgEl) {
          const char* imgSrc = imgEl->Attribute("source");
          if (imgSrc) {
            std::string imgPath = tmxDir + imgSrc;
            assetManager->addTexture(renderer, "tilemap", imgPath);
          }
        }
      }
    }
  }

  // ── Tile layers ───────────────────────────────────────────────────────────
  int layerIndex = 0;
  for (auto* xmlLayer = xmlRoot->FirstChildElement("layer");
       xmlLayer != nullptr;
       xmlLayer = xmlLayer->NextSiblingElement("layer")) {
    loadLayer(registry, xmlLayer, tWidth, tHeight, mWidth, mHeight,
              tilesetColumns, renderer, assetManager, layerIndex++);
  }

  // ── Object groups (paredes, obstáculos, jugador, etc.) ────────────────────
  for (auto* objGroup = xmlRoot->FirstChildElement("objectgroup");
       objGroup != nullptr;
       objGroup = objGroup->NextSiblingElement("objectgroup")) {
    loadObjectGroup(lua, registry, objGroup);
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// loadLayer  (tile layer)
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadLayer(std::unique_ptr<Registry>& registry,
                            tinyxml2::XMLElement* xmlLayer,
                            int tWidth, int tHeight, int mWidth, int mHeight,
                            int tilesetColumns,
                            SDL_Renderer* renderer,
                            std::unique_ptr<AssetManager>& assetManager,
                            int layerIndex) {
  tinyxml2::XMLElement* dataEl = xmlLayer->FirstChildElement("data");
  if (!dataEl) return;
  const char* rawData = dataEl->GetText();
  if (!rawData) return;

  SDL_Texture* tileset = assetManager->getTexture("tilemap");
  if (!tileset) return;

  // Crear textura destino del tamaño total del mapa
  const int mapPixelW = mWidth  * tWidth;
  const int mapPixelH = mHeight * tHeight;

  SDL_Texture* baked = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET,
                                          mapPixelW, mapPixelH);
  if (!baked) return;

  SDL_SetTextureBlendMode(baked, SDL_BLENDMODE_NONE);
  SDL_SetRenderTarget(renderer, baked);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  std::istringstream stream(rawData);
  std::string token;
  int col = 0, row = 0;

  while (std::getline(stream, token, ',')) {
    token.erase(0, token.find_first_not_of(" \t\r\n"));
    if (token.empty()) continue;
    int tileId = std::stoi(token);

    if (tileId != 0) {
      int srcX = ((tileId - 1) % tilesetColumns) * tWidth;
      int srcY = ((tileId - 1) / tilesetColumns) * tHeight;
      SDL_Rect src = { srcX, srcY, tWidth, tHeight };
      SDL_Rect dst = { col * tWidth, row * tHeight, tWidth, tHeight };
      SDL_RenderCopy(renderer, tileset, &src, &dst);
    }

    col++;
    if (col >= mWidth) { col = 0; row++; }
  }

  SDL_SetRenderTarget(renderer, nullptr);

  // Registrar la textura horneada y crear una sola entidad
  std::string bakedId = "baked_layer_" + std::to_string(layerIndex);
  assetManager->addRawTexture(bakedId, baked);

  Entity layer = registry->createEntity();
  layer.addComponent<TransformComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0);
  layer.addComponent<SpriteComponent>(bakedId, mapPixelW, mapPixelH, 0, 0, false);
}

// ─────────────────────────────────────────────────────────────────────────────
// loadObjectGroup  (Object Layer de Tiled → entidades ECS)
// Clases soportadas: wall, ramp, gap, saw, score_zone, checkpoint, player_spawn
// ─────────────────────────────────────────────────────────────────────────────

void SceneLoader::loadObjectGroup(sol::state& lua,
                                  std::unique_ptr<Registry>& registry,
                                  tinyxml2::XMLElement* objGroup) {
  // Crea una score_zone invisible de ancho total del mapa encima de una saw
  auto spawnScoreZone = [&](float sawY) {
    float fullW = static_cast<float>(Game::getInstance().mapWidth);
    if (fullW <= 0) fullW = 800.0f;
    Entity sz = registry->createEntity();
    sz.addComponent<TransformComponent>(glm::vec2(0.0f, sawY - 5.0f), glm::vec2(1,1), 0.0);
    sz.addComponent<BoxColliderComponent>(fullW, 10.0f);
    sz.addComponent<TagComponent>("score_zone");
    lua["on_click"] = sol::lua_nil;
    lua["update"]   = sol::lua_nil;
    lua.script_file("./assets/scripts/score_zone.lua");
    sz.addComponent<ScriptComponent>(lua["update"], sol::lua_nil);
  };

  for (auto* obj = objGroup->FirstChildElement("object");
       obj != nullptr;
       obj = obj->NextSiblingElement("object")) {

    const char* classAttr = obj->Attribute("class");
    if (!classAttr) classAttr = obj->Attribute("type"); // compatibilidad Tiled < 1.9
    if (!classAttr) continue;
    std::string cls = classAttr;

    float x = 0, y = 0, w = 16, h = 16;
    obj->QueryFloatAttribute("x",      &x);
    obj->QueryFloatAttribute("y",      &y);
    obj->QueryFloatAttribute("width",  &w);
    obj->QueryFloatAttribute("height", &h);

    // Leer custom properties del objeto
    auto getprop = [&](const std::string& name, const std::string& def) -> std::string {
      auto* propsEl = obj->FirstChildElement("properties");
      if (!propsEl) return def;
      for (auto* p = propsEl->FirstChildElement("property"); p; p = p->NextSiblingElement("property")) {
        const char* n = p->Attribute("name");
        const char* v = p->Attribute("value");
        if (n && v && std::string(n) == name) return v;
      }
      return def;
    };

    Entity e = registry->createEntity();

    if (cls == "wall") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("wall");
    }
    else if (cls == "ramp") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("ramp");
    }
    else if (cls == "gap") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("gap");
    }
    else if (cls == "saw") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("saw");
      if (getprop("spin", "false") == "true") {
        lua["on_click"] = sol::lua_nil;
        lua["update"]   = sol::lua_nil;
        lua.script_file("./assets/scripts/saw_spin.lua");
        sol::function updateFn = lua["update"];
        e.addComponent<ScriptComponent>(updateFn, sol::lua_nil);
      }
      spawnScoreZone(y);
    }
    else if (cls == "patrol_saw") {
      // Pasar propiedades de Tiled al script antes de cargarlo
      lua["patrol_axis"]  = getprop("patrol_axis",  "y");
      lua["patrol_range"] = std::stof(getprop("patrol_range", "80"));
      lua["patrol_speed"] = std::stof(getprop("patrol_speed", "60"));

      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<SpriteComponent>("saw_sprite", (int)w, (int)h, 0, 0, false);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("saw");
      lua["on_click"] = sol::lua_nil;
      lua["update"]   = sol::lua_nil;
      lua.script_file("./assets/scripts/saw_patrol.lua");
      e.addComponent<ScriptComponent>(lua["update"], sol::lua_nil);
      spawnScoreZone(y);
    }
    else if (cls == "score_zone") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("score_zone");
      lua["on_click"] = sol::lua_nil;
      lua["update"]   = sol::lua_nil;
      lua.script_file("./assets/scripts/score_zone.lua");
      sol::function updateFn = lua["update"];
      e.addComponent<ScriptComponent>(updateFn, sol::lua_nil);
    }
    else if (cls == "checkpoint") {
      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1, 1), 0.0);
      e.addComponent<BoxColliderComponent>(w, h);
      e.addComponent<TagComponent>("checkpoint");
    }
    else if (cls == "player_spawn") {
      float speed    = std::stof(getprop("base_speed",      "220"));
      float rotSpeed = std::stof(getprop("rotation_speed",  "130"));
      float scaleX   = std::stof(getprop("scale",           "0.35"));
      float scaleY   = scaleX;

      e.addComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(scaleX, scaleY), 0.0);
      e.addComponent<SpriteComponent>("car_yellow_sprite", 71, 131, 0, 0, false);
      e.addComponent<RigidBodyComponent>(glm::vec2(0, 0));
      e.addComponent<BoxColliderComponent>(71, 131);
      e.addComponent<CircleColliderComponent>(35, 71, 131);
      e.addComponent<TagComponent>("player");
      e.addComponent<PlayerComponent>(speed, rotSpeed, 0.30f, 2.0f, scaleX, scaleY);
      e.getComponent<PlayerComponent>().spawnX = x;
      e.getComponent<PlayerComponent>().spawnY = y;
      e.addComponent<NitroComponent>(0.5f, 0.5f);
      lua["on_click"] = sol::lua_nil;
      lua["update"]   = sol::lua_nil;
      lua.script_file("./assets/scripts/player_car.lua");
      sol::function updateFn = lua["update"];
      e.addComponent<ScriptComponent>(updateFn, sol::lua_nil);
    }
    else {
      // Clase desconocida: liberar entidad vacía sin error fatal
      registry->destroyEntity(e);
    }
  }
}
