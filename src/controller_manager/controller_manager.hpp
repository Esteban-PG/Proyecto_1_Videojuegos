#ifndef CONTROLLER_MANAGER_HPP
#define CONTROLLER_MANAGER_HPP

#include <SDL2/SDL.h>

#include <map>
#include <string>
#include <tuple>
#include <vector>

/**
 * @brief Maps raw SDL input events to named game actions and button states.
 *
 * Keyboard keys are grouped into named actions (e.g. "accelerate", "nitro")
 * so that multiple keys can trigger the same logical action.
 * Mouse buttons are registered by name for use in UI click detection.
 */
class ControllerManager {
 private:
  std::map<std::string, std::vector<int>> actionKeyName;
  std::map<int, bool> keyStates;

  std::map<std::string, int> mouseButtonName;
  std::map<int, bool> mouseButtonStates;
  int mousePosX;
  int mousePosY;

 public:
  ControllerManager();
  ~ControllerManager();

  /** @brief Remove all key and mouse button bindings. */
  void clear();

  /** @brief Bind a SDL key code to a named action. Multiple keys may share one action. */
  void addActionKey(const std::string& action, int keyCode);
  /** @brief Mark a key as pressed. Called from Game::processInput(). */
  void keyDown(int keyCode);
  /** @brief Mark a key as released. Called from Game::processInput(). */
  void keyUp(int keyCode);
  /** @brief @return True while any key bound to @p action is held down. */
  bool isActionActivated(const std::string& action);

  /** @brief Register a mouse button code under a named identifier. */
  void addMouseButton(const std::string& name, int buttonCode);
  /** @brief Mark a mouse button as pressed. */
  void mouseButtonDown(int buttonCode);
  /** @brief Mark a mouse button as released. */
  void mouseButtonUp(int buttonCode);
  /** @brief @return True while the named mouse button is held. */
  bool isMouseButtonDown(const std::string& name);

  /** @brief Update the cached mouse cursor position. */
  void setMousePosition(int x, int y);
  /** @brief @return Current mouse position as (x, y). */
  std::tuple<int, int> getMousePosition();
};

#endif