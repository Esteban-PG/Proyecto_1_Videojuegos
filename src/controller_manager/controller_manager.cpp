#include "controller_manager.hpp"

#include <iostream>

ControllerManager::ControllerManager() {
  // Constructor
  std::cout << "[CONTROLLER_MANAGER] Constructor called" << std::endl;
}

ControllerManager::~ControllerManager() {
  // Destructor
  std::cout << "[CONTROLLER_MANAGER] Destructor called" << std::endl;
}

void ControllerManager::clear() {
  actionKeyName.clear();
  keyStates.clear();
}

void ControllerManager::addActionKey(const std::string& action, int keyCode) {
  actionKeyName[action].push_back(keyCode);
  if (keyStates.find(keyCode) == keyStates.end()) {
    keyStates.emplace(keyCode, false);
  }
}

void ControllerManager::keyDown(int keyCode) {
  auto it = keyStates.find(keyCode);
  if (it != keyStates.end()) {
    keyStates[keyCode] = true;
  }
}

void ControllerManager::keyUp(int keyCode) {
  auto it = keyStates.find(keyCode);
  if (it != keyStates.end()) {
    keyStates[keyCode] = false;
  }
}

bool ControllerManager::isActionActivated(const std::string& action) {
  auto it = actionKeyName.find(action);
  if (it != actionKeyName.end()) {
    for (int keyCode : it->second) {
      if (keyStates[keyCode]) {
        return true;
      }
    }
  }
  return false;
}

void ControllerManager::addMouseButton(const std::string& name,
                                       int buttonCode) {
  mouseButtonName.emplace(name, buttonCode);
  mouseButtonStates.emplace(buttonCode, false);
}

void ControllerManager::mouseButtonDown(int buttonCode) {
  auto it = mouseButtonStates.find(buttonCode);
  if (it != mouseButtonStates.end()) {
    mouseButtonStates[buttonCode] = true;
  }
}

void ControllerManager::mouseButtonUp(int buttonCode) {
  auto it = mouseButtonStates.find(buttonCode);
  if (it != mouseButtonStates.end()) {
    mouseButtonStates[buttonCode] = false;
  }
}

bool ControllerManager::isMouseButtonDown(const std::string& name) {
  auto it = mouseButtonName.find(name);
  if (it != mouseButtonName.end()) {
    int buttonCode = mouseButtonName[name];
    return mouseButtonStates[buttonCode];
  }
  return false;
}

void ControllerManager::setMousePosition(int x, int y) {
  mousePosX = x;
  mousePosY = y;
}
std::tuple<int, int> ControllerManager::getMousePosition() {
  return std::make_tuple(mousePosX, mousePosY);
}
