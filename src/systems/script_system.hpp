#ifndef SCRIPT_SYSTEM_HPP
#define SCRIPT_SYSTEM_HPP

#include <memory>
#include <sol/sol.hpp>

#include "../ECS/ECS.hpp"
#include "../binding/lua_binding.hpp"
#include "../components/script_component.hpp"

/**
 * @brief Runs Lua update()/on_click() functions attached to entities.
 *
 * createLuaBinding() must be called once at startup to expose C++ types and
 * helper functions to the Lua VM before any scene is loaded.
 *
 * Each frame update() sets `lua["this"]` to the current entity so scripts can
 * call helpers that take an entity argument (isPlayerDead(this), etc.).
 *
 * The global `delta_time` is set by game.cpp before calling update() so
 * scripts can do frame-rate-independent work (e.g. saw rotation).
 */
class ScriptSystem : public System {
 public:
  ScriptSystem() { requireComponent<ScriptComponent>(); }
  ~ScriptSystem() = default;

  /** Bind all C++ helpers and the Entity usertype to the Lua state. */
  void createLuaBinding(sol::state& lua) {
    // Entity usertype — Lua scripts receive entity objects via `this`
    lua.new_usertype<Entity>("entity");

    // ── Input ──────────────────────────────────────────────────────────────
    lua.set_function("isActionActivated", isActionActivated);

    // ── Transform ─────────────────────────────────────────────────────────
    lua.set_function("getEntityPosition", getEntityPosition);
    lua.set_function("setEntityPosition", setEntityPosition);
    lua.set_function("getEntityRotation", getEntityRotation);
    lua.set_function("setEntityRotation", setEntityRotation);

    // ── RigidBody ─────────────────────────────────────────────────────────
    lua.set_function("setVelocity", setVelocity);
    lua.set_function("getVelocity", getVelocity);

    // ── Player ────────────────────────────────────────────────────────────
    lua.set_function("getPlayerPosition", getPlayerPosition);
    lua.set_function("isPlayerDead",      isPlayerDead);
    lua.set_function("isPlayerWon",       isPlayerWon);
    lua.set_function("addScore",          addScore);
    lua.set_function("getScore",          getScore);

    // ── Text ──────────────────────────────────────────────────────────────
    lua.set_function("setTextContent", setTextContent);

    // ── Audio ─────────────────────────────────────────────────────────────
    lua.set_function("playSFX",   playSFX);
    lua.set_function("playMusic", playMusic);
    lua.set_function("stopMusic", stopMusic);

    // ── Scene ─────────────────────────────────────────────────────────────
    lua.set_function("goToScene",  goToScene);
    lua.set_function("quitGame",   quitGame);
  }

  /** Execute each entity's Lua update() function. */
  void update(sol::state& lua) {
    for (Entity entity : getEntities()) {
      const auto& script = entity.getComponent<ScriptComponent>();
      if (script.update != sol::lua_nil) {
        lua["this"] = entity;
        script.update();
      }
    }
  }
};

#endif  // SCRIPT_SYSTEM_HPP
