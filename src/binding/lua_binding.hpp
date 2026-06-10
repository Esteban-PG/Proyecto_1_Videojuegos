#ifndef LUA_BINDING_HPP
#define LUA_BINDING_HPP

#include <string>
#include <tuple>

#include "../components/animation_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/text_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"
#include "../systems/tag_system.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Input
// ─────────────────────────────────────────────────────────────────────────────

bool isActionActivated(const std::string& action) {
  return Game::getInstance().controllerManager->isActionActivated(action);
}

// ─────────────────────────────────────────────────────────────────────────────
// RigidBody helpers
// ─────────────────────────────────────────────────────────────────────────────

void setVelocity(Entity entity, float x, float y) {
  auto& rb = entity.getComponent<RigidBodyComponent>();
  rb.velocity.x = x;
  rb.velocity.y = y;
}

std::tuple<float, float> getVelocity(Entity entity) {
  const auto& rb = entity.getComponent<RigidBodyComponent>();
  return {rb.velocity.x, rb.velocity.y};
}

// ─────────────────────────────────────────────────────────────────────────────
// Transform helpers
// ─────────────────────────────────────────────────────────────────────────────

std::tuple<float, float> getEntityPosition(Entity entity) {
  const auto& tf = entity.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

void setEntityPosition(Entity entity, float x, float y) {
  auto& tf = entity.getComponent<TransformComponent>();
  tf.position.x = x;
  tf.position.y = y;
}

float getEntityRotation(Entity entity) {
  return static_cast<float>(entity.getComponent<TransformComponent>().rotation);
}

void setEntityRotation(Entity entity, float deg) {
  entity.getComponent<TransformComponent>().rotation = deg;
}

// ─────────────────────────────────────────────────────────────────────────────
// Player-specific helpers
// ─────────────────────────────────────────────────────────────────────────────

std::tuple<float, float> getPlayerPosition() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() < 0) return {0.0f, 0.0f};
  const auto& tf = p.getComponent<TransformComponent>();
  return {tf.position.x, tf.position.y};
}

bool isPlayerDead(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().isDead;
}

bool isPlayerWon(Entity entity) {
  if (!entity.hasComponent<PlayerComponent>()) return false;
  return entity.getComponent<PlayerComponent>().hasWon;
}

void addScore(int value) {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() >= 0 && p.hasComponent<PlayerComponent>()) {
    p.getComponent<PlayerComponent>().score += value;
  }
}

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

void setTextContent(Entity entity, const std::string& text) {
  if (entity.hasComponent<TextComponent>()) {
    entity.getComponent<TextComponent>().text = text;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Audio helpers
// ─────────────────────────────────────────────────────────────────────────────

void playSFX(const std::string& id) {
  Game::getInstance().audioManager->playSFX(id);
}

void playMusic(const std::string& id) {
  Game::getInstance().audioManager->playMusic(id);
}

void stopMusic() {
  Game::getInstance().audioManager->stopMusic();
}

// ─────────────────────────────────────────────────────────────────────────────
// Scene navigation
// ─────────────────────────────────────────────────────────────────────────────

void respawnPlayer() {
  auto& tagSys = Game::getInstance().registry->getSystem<TagSystem>();
  Entity p = tagSys.getEntityByTag("player");
  if (p.getId() < 0) return;

  auto& tf = p.getComponent<TransformComponent>();
  auto& pc = p.getComponent<PlayerComponent>();
  auto& rb = p.getComponent<RigidBodyComponent>();

  tf.position = {pc.spawnX, pc.spawnY};
  tf.scale    = {pc.baseScaleX, pc.baseScaleY};
  tf.rotation = 0.0;

  rb.velocity  = {0.0f, 0.0f};
  pc.velocityY = 0.0f;
  pc.isOnGround = false;
  pc.canJump    = false;
  pc.score      = 0;
  pc.isDead     = false;
  pc.hasWon     = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Animation helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Switch to a new animation strip on an entity.
 *
 * Does nothing if the entity already has this animation playing.
 * Resets startTime so the new animation begins from frame 0.
 *
 * @param numFrames    Total frames across all rows
 * @param framesPerRow Frames per row in the sprite sheet
 * @param fps          Playback speed in frames per second
 * @param isLoop       If false, the animation stops at the last frame
 */
void playAnimation(Entity entity, const std::string& assetId,
                   int numFrames, int framesPerRow, int fps, bool isLoop, int row) {
  if (!entity.hasComponent<SpriteComponent>())    return;
  if (!entity.hasComponent<AnimationComponent>()) return;

  auto& sp = entity.getComponent<SpriteComponent>();
  auto& an = entity.getComponent<AnimationComponent>();

  // Only reset when something actually changed
  if (sp.textureId == assetId && an.isLoop == isLoop && an.baseRow == row) return;

  sp.textureId  = assetId;
  sp.srcRect.x  = 0;
  sp.srcRect.y  = 0;

  an.numberOfFrames = numFrames;
  an.framesPerRow   = framesPerRow;
  an.baseRow        = row;
  an.frameSpeedRate = fps;
  an.isLoop         = isLoop;
  an.currentFrame   = 0;
  an.startTime      = SDL_GetTicks();
}

/**
 * @brief Horizontally mirror a sprite (e.g. player facing left).
 * @param flip True to flip, false to restore normal orientation
 */
void setFlipX(Entity entity, bool flip) {
  if (entity.hasComponent<SpriteComponent>())
    entity.getComponent<SpriteComponent>().flipX = flip;
}

void goToScene(const std::string& sceneName) {
  Game::getInstance().sceneManager->setNextScene(sceneName);
  Game::getInstance().sceneManager->stopScene();
}

void quitGame() {
  Game::getInstance().quit();
}

#endif  // LUA_BINDING_HPP
