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

/** Returns true while the named action's key(s) are held. */
bool isActionActivated(const std::string& action) {
  return Game::getInstance().controllerManager->isActionActivated(action);
}

// ─────────────────────────────────────────────────────────────────────────────
// RigidBody helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Override an entity's velocity directly (pixels/second). */
void setVelocity(Entity entity, float x, float y) {
  auto& rb = entity.getComponent<RigidBodyComponent>();
  rb.velocity.x = x;
  rb.velocity.y = y;
}

/** Return (vx, vy) for the entity. */
std::tuple<float, float> getVelocity(Entity entity) {
  const auto& rb = entity.getComponent<RigidBodyComponent>();
  return {rb.velocity.x, rb.velocity.y};
}

// ─────────────────────────────────────────────────────────────────────────────
// Transform helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Return (x, y) world position of an entity (top-left corner of its AABB). */
std::tuple<float, float> getEntityPosition(Entity entity) {
  const auto& tf = entity.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

/** Teleport an entity to world position (x, y). */
void setEntityPosition(Entity entity, float x, float y) {
  auto& tf = entity.getComponent<TransformComponent>();
  tf.position.x = x;
  tf.position.y = y;
}

/** Return the entity's current rotation in degrees. */
float getEntityRotation(Entity entity) {
  return static_cast<float>(entity.getComponent<TransformComponent>().rotation);
}

/** Set the entity's rotation in degrees. */
void setEntityRotation(Entity entity, float deg) {
  entity.getComponent<TransformComponent>().rotation = deg;
}

// ─────────────────────────────────────────────────────────────────────────────
// Player-specific helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Return (x, y) of the entity tagged "player", or (0, 0) if not found. */
std::tuple<float, float> getPlayerPosition() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() < 0) return {0.0f, 0.0f};
  const auto& tf = p.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

/** True if the player entity's PlayerComponent.isDead flag is set. */
bool isPlayerDead(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().isDead;
}

/** True if the player entity's PlayerComponent.hasWon flag is set. */
bool isPlayerWon(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().hasWon;
}

/** Add points to the player entity's score (found via TagSystem). */
void addScore(int value) {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() >= 0 && p.hasComponent<PlayerComponent>()) {
    p.getComponent<PlayerComponent>().score += value;
  }
}

/** Return the player's current score. */
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

/** Replace the text displayed by a TextComponent entity. */
void setTextContent(Entity entity, const std::string& text) {
  if (entity.hasComponent<TextComponent>()) {
    entity.getComponent<TextComponent>().text = text;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Audio helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Play a loaded sound-effect by its asset ID. */
void playSFX(const std::string& id) {
  Game::getInstance().audioManager->playSFX(id);
}

/** Start (or restart) a music track by its asset ID. */
void playMusic(const std::string& id) {
  Game::getInstance().audioManager->playMusic(id);
}

/** Fade out any currently playing music. */
void stopMusic() {
  Game::getInstance().audioManager->stopMusic();
}

// ─────────────────────────────────────────────────────────────────────────────
// Scene navigation
// ─────────────────────────────────────────────────────────────────────────────

/** Resetea al jugador en su punto de spawn sin recargar la escena. */
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

/** Transition to a different scene by name (defined in scenes.lua). */
void goToScene(const std::string& sceneName) {
  Game::getInstance().sceneManager->setNextScene(sceneName);
  Game::getInstance().sceneManager->stopScene();
}

/** Exit the application cleanly from a Lua script. */
void quitGame() {
  Game::getInstance().quit();
}

#endif  // LUA_BINDING_HPP
