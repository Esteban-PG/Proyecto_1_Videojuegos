#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <sol/sol.hpp>

/**
 * @brief Attaches Lua callback functions to an entity.
 *
 * ScriptSystem calls update() every frame and UISystem calls onClick()
 * when a ClickEvent lands inside the entity's text bounds.
 * Either function may be sol::lua_nil if not needed.
 */
struct ScriptComponent {
  sol::function update;   ///< Called every frame by ScriptSystem
  sol::function onClick;  ///< Called on left-click by UISystem

  ScriptComponent(sol::function update = sol::lua_nil,
                  sol::function onClick = sol::lua_nil)
      : update(update), onClick(onClick) {}
};

#endif  // SCRIPT_COMPONENT_HPP