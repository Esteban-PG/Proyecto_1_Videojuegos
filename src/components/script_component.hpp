#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <sol/sol.hpp>

struct ScriptComponent {
  sol::function update;
  sol::function onClick;

  ScriptComponent(sol::function update = sol::lua_nil,
                  sol::function onClick = sol::lua_nil)
      : update(update), onClick(onClick) {}
};

#endif  // SCRIPT_COMPONENT_HPP