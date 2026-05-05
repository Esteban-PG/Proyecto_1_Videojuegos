#include "scene_manager.hpp"

#include <iostream>

#include "../game/game.hpp"

SceneManager::SceneManager() {
  std::cout << "[SCENE MANAGER] Constructor called." << std::endl;
  sceneLoader = std::make_unique<SceneLoader>();
}

SceneManager::~SceneManager() {
  std::cout << "[SCENE MANAGER] Destructor called." << std::endl;
  sceneLoader.reset();
}

void SceneManager::loadScenesFromScript(const std::string& path,
                                        sol::state& lua) {
  lua.script_file(path);
  sol::table scenes = lua["scenes"];

  int index = 0;
  while (true) {
    sol::optional<sol::table> hasScene = scenes[index];
    if (hasScene == sol::nullopt) {
      break;
    }
    sol::table scene = scenes[index];

    this->scenes.emplace(scene["name"], scene["path"]);

    if (index == 0) {
      nextScene = scene["name"];
    }
    index++;
  }
}

void SceneManager::loadScene() {
  std::cout << "[SCENE MANAGER] loadScene nextScene: " << nextScene << " sceneFilePath: " << scenes[nextScene] << std::endl;
  Game& game = Game::getInstance();
  std::string sceneFilePath = scenes[nextScene];
  sceneLoader->loadScene(sceneFilePath, game.lua, game.assetManager,
                         game.controllerManager, game.registry, game.renderer);
}

std::string SceneManager::getNextScene() const { return nextScene; }

void SceneManager::setNextScene(const std::string& nextScene) {
  this->nextScene = nextScene;
}

bool SceneManager::isSceneRunning() const { return isRunning; }

void SceneManager::startScene() { isRunning = true; }

void SceneManager::stopScene() { isRunning = false; }