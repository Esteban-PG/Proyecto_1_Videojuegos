#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <map>
#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "scene_loader.hpp"

/**
 * @brief Owns the scene registry and drives scene transitions.
 *
 * Scenes are registered from a Lua script (scenes.lua) at startup.
 * The game loop calls startScene() → loadScene() → isSceneRunning()
 * in a loop; stopScene() signals the loop to end and triggers a transition.
 */
class SceneManager {
 private:
  std::map<std::string, std::string> scenes;
  std::string nextScene;
  bool isRunning = false;
  std::unique_ptr<SceneLoader> sceneLoader;

 public:
  SceneManager();
  ~SceneManager();

  /** @brief Parse scenes.lua and populate the name→path registry. */
  void loadScenesFromScript(const std::string& path, sol::state& lua);
  /** @brief Instantiate the nextScene using SceneLoader. */
  void loadScene();

  /** @brief @return Name of the scene that will load next. */
  std::string getNextScene() const;
  /** @brief Queue a scene transition by name. */
  void setNextScene(const std::string& nextScene);

  /** @brief @return True while the current scene is active. */
  bool isSceneRunning() const;
  /** @brief Mark the current scene as active (isRunning = true). */
  void startScene();
  /** @brief Signal the current scene to end on the next loop iteration. */
  void stopScene();
};

#endif  // SCENE_MANAGER_HPP