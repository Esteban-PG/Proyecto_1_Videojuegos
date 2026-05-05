#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <map>
#include <memory>
#include <sol/sol.hpp>
#include <string>

#include "scene_loader.hpp"

class SceneManager {
 private:
  std::map<std::string, std::string> scenes;
  std::string nextScene;
  bool isRunning = false;
  std::unique_ptr<SceneLoader> sceneLoader;

 public:
  SceneManager();
  ~SceneManager();

  void loadScenesFromScript(const std::string& path, sol::state& lua);
  void loadScene();

  std::string getNextScene() const;
  void setNextScene(const std::string& nextScene);

  bool isSceneRunning() const;
  void startScene();
  void stopScene();
};

#endif  // SCENE_MANAGER_HPP