#ifndef LUA_BINDING_HPP
#define LUA_BINDING_HPP

#include <string>
#include <tuple>

#include "../components/nitro_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/text_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"
#include "../systems/tag_system.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Input
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Check whether a named input action is currently held.
 * @param action Action name registered in the scene (e.g. "accelerate")
 * @return True while any key bound to @p action is pressed
 */
bool isActionActivated(const std::string& action) {
  return Game::getInstance().controllerManager->isActionActivated(action);
}

// ─────────────────────────────────────────────────────────────────────────────
// RigidBody helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Set an entity's velocity directly.
 * @param entity Target entity (must have RigidBodyComponent)
 * @param x Horizontal velocity in pixels/second
 * @param y Vertical velocity in pixels/second
 */
void setVelocity(Entity entity, float x, float y) {
  auto& rb = entity.getComponent<RigidBodyComponent>();
  rb.velocity.x = x;
  rb.velocity.y = y;
}

/**
 * @brief Get an entity's current velocity.
 * @param entity Target entity (must have RigidBodyComponent)
 * @return (vx, vy) in pixels/second
 */
std::tuple<float, float> getVelocity(Entity entity) {
  const auto& rb = entity.getComponent<RigidBodyComponent>();
  return {rb.velocity.x, rb.velocity.y};
}

// ─────────────────────────────────────────────────────────────────────────────
// Transform helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Get an entity's world position (top-left of its bounding box).
 * @param entity Target entity (must have TransformComponent)
 * @return (x, y) world position in pixels
 */
std::tuple<float, float> getEntityPosition(Entity entity) {
  const auto& tf = entity.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

/**
 * @brief Teleport an entity to a world position.
 * @param entity Target entity (must have TransformComponent)
 * @param x World X in pixels
 * @param y World Y in pixels
 */
void setEntityPosition(Entity entity, float x, float y) {
  auto& tf = entity.getComponent<TransformComponent>();
  tf.position.x = x;
  tf.position.y = y;
}

/**
 * @brief Get the entity's current rotation.
 * @param entity Target entity (must have TransformComponent)
 * @return Rotation in degrees (clockwise)
 */
float getEntityRotation(Entity entity) {
  return static_cast<float>(entity.getComponent<TransformComponent>().rotation);
}

/**
 * @brief Set the entity's rotation.
 * @param entity Target entity (must have TransformComponent)
 * @param deg Rotation in degrees (clockwise)
 */
void setEntityRotation(Entity entity, float deg) {
  entity.getComponent<TransformComponent>().rotation = deg;
}

// ─────────────────────────────────────────────────────────────────────────────
// Player-specific helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Get the world position of the entity tagged "player".
 * @return (x, y) world position, or (0, 0) if no player entity exists
 */
std::tuple<float, float> getPlayerPosition() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() < 0) return {0.0f, 0.0f};
  const auto& tf = p.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

/**
 * @brief Check whether the player is dead.
 * @param entity Player entity handle
 * @return True if PlayerComponent::isDead is set
 */
bool isPlayerDead(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().isDead;
}

/**
 * @brief Check whether the player has reached the checkpoint.
 * @param entity Player entity handle
 * @return True if PlayerComponent::hasWon is set
 */
bool isPlayerWon(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().hasWon;
}

/**
 * @brief Add @p value points to the player's score.
 * @param value Points to add (may be negative)
 */
void addScore(int value) {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() >= 0 && p.hasComponent<PlayerComponent>()) {
    p.getComponent<PlayerComponent>().score += value;
  }
}

/**
 * @brief Get the player's accumulated score.
 * @return Current score, or 0 if no player entity exists
 */
int getScore() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() >= 0 && p.hasComponent<PlayerComponent>())
    return p.getComponent<PlayerComponent>().score;
  return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Text helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Update the string rendered by a TextComponent.
 * @param entity Target entity (must have TextComponent)
 * @param text   New text string to display
 */
void setTextContent(Entity entity, const std::string& text) {
  if (entity.hasComponent<TextComponent>()) {
    entity.getComponent<TextComponent>().text = text;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Audio helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Play a one-shot sound effect.
 * @param id Asset ID registered with AudioManager::addSFX()
 */
void playSFX(const std::string& id) {
  Game::getInstance().audioManager->playSFX(id);
}

/**
 * @brief Start looping a music track.
 * @param id Asset ID registered with AudioManager::addMusic()
 */
void playMusic(const std::string& id) {
  Game::getInstance().audioManager->playMusic(id);
}

/** @brief Fade out any currently playing music track. */
void stopMusic() {
  Game::getInstance().audioManager->stopMusic();
}

// ─────────────────────────────────────────────────────────────────────────────
// Scene navigation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Teleport the player back to its spawn point and reset all state.
 *
 * Does NOT reload the scene — position, velocity, isDead, isFalling and
 * isJumping are all reset in-place so the level continues running.
 */
void respawnPlayer() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() < 0) return;

  auto& tf = p.getComponent<TransformComponent>();
  auto& pc = p.getComponent<PlayerComponent>();
  auto& rb = p.getComponent<RigidBodyComponent>();

  // Restaurar posición al spawn original
  tf.position = {pc.spawnX, pc.spawnY};
  tf.scale    = {pc.baseScaleX, pc.baseScaleY};
  tf.rotation = 0.0;

  // Resetear estado del jugador
  rb.velocity  = {0.0f, 0.0f};
  pc.isDead    = false;
  pc.hasWon    = false;
  pc.isFalling = false;
  pc.fallTimer = 0.0f;
  pc.isJumping = false;
  pc.jumpTimer = 0.0f;
}

/**
 * @brief Transition to a named scene at the end of the current frame.
 * @param sceneName Name registered in scenes.lua (e.g. "nitro_menu")
 */
void goToScene(const std::string& sceneName) {
  Game::getInstance().sceneManager->setNextScene(sceneName);
  Game::getInstance().sceneManager->stopScene();
}

/** @brief Exit the application cleanly from a Lua script. */
void quitGame() {
  Game::getInstance().quit();
}

#endif  // LUA_BINDING_HPP
