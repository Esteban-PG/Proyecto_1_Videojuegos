#ifndef BOX_COLLISION_SYSTEM_HPP
#define BOX_COLLISION_SYSTEM_HPP

#include <algorithm>
#include <string>

#include "../ECS/ECS.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/tag_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"
#include "../systems/tag_system.hpp"

/**
 * @brief AABB collision between the player and all world-geometry entities.
 *
 * Responses per tag:
 *   "wall" / "platform" → push-out on min-overlap axis; landing from above
 *                          sets isOnGround/canJump and zeros velocityY.
 *   "saw"               → instant death.
 *   "checkpoint"        → sets hasWon.
 *   "trigger"           → no physical response (handled by Lua scripts).
 */
class BoxCollisionSystem : public System {
 public:
  BoxCollisionSystem() {
    requireComponent<BoxColliderComponent>();
    requireComponent<TransformComponent>();
    requireComponent<TagComponent>();
  }

  void update() {
    auto& tagSystem   = Game::getInstance().registry->getSystem<TagSystem>();
    Entity playerEntity = tagSystem.getEntityByTag("player");
    if (playerEntity.getId() < 0) return;
    if (!playerEntity.hasComponent<PlayerComponent>()) return;

    auto& playerTransform = playerEntity.getComponent<TransformComponent>();
    auto& playerBox       = playerEntity.getComponent<BoxColliderComponent>();
    auto& playerComp      = playerEntity.getComponent<PlayerComponent>();

    if (playerComp.isDead || playerComp.hasWon) return;

    const float pw = playerBox.width  * playerTransform.scale.x;
    const float ph = playerBox.height * playerTransform.scale.y;

    for (Entity entity : getEntities()) {
      if (entity == playerEntity) continue;

      const auto& tag       = entity.getComponent<TagComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();
      const auto& box       = entity.getComponent<BoxColliderComponent>();

      const float ew = box.width  * transform.scale.x;
      const float eh = box.height * transform.scale.y;
      const float ex = transform.position.x;
      const float ey = transform.position.y;

      // Re-read player position after earlier corrections this frame
      const float px = playerTransform.position.x;
      const float py = playerTransform.position.y;

      if (!overlaps(px, py, pw, ph, ex, ey, ew, eh)) continue;

      handleCollision(tag.tag, playerEntity, playerTransform, playerComp,
                      px, py, pw, ph, ex, ey, ew, eh);
    }
  }

 private:
  static bool overlaps(float ax, float ay, float aw, float ah,
                        float bx, float by, float bw, float bh) {
    return ax < bx + bw && ax + aw > bx &&
           ay < by + bh && ay + ah > by;
  }

  void handleCollision(const std::string& tag,
                        Entity playerEntity,
                        TransformComponent& tf, PlayerComponent& pc,
                        float px, float py, float pw, float ph,
                        float ex, float ey, float ew, float eh) {
    if (tag == "wall" || tag == "platform") {
      resolvePlatformCollision(playerEntity, tf, pc,
                               px, py, pw, ph, ex, ey, ew, eh);
    } else if (tag == "saw") {
      if (!pc.isDead) {
        pc.isDead = true;
        Game::getInstance().audioManager->playSFX("death");
        Game::getInstance().triggerShake(0.25f, 7.0f);
      }
    } else if (tag == "checkpoint") {
      if (!pc.hasWon) {
        pc.hasWon = true;
        Game::getInstance().audioManager->playSFX("win");
      }
    }
    // "trigger" — scripts handle the flag, no physics needed
  }

  void resolvePlatformCollision(Entity playerEntity,
                                 TransformComponent& tf, PlayerComponent& pc,
                                 float px, float py, float pw, float ph,
                                 float ex, float ey, float ew, float eh) {
    const float overlapRight  = (px + pw) - ex;
    const float overlapLeft   = (ex + ew) - px;
    const float overlapBottom = (py + ph) - ey;
    const float overlapTop    = (ey + eh) - py;

    const float minX = std::min(overlapRight, overlapLeft);
    const float minY = std::min(overlapBottom, overlapTop);

    if (minY < minX) {
      if (overlapBottom < overlapTop) {
        // Player's bottom hit the top surface — landed from above
        tf.position.y -= overlapBottom;
        pc.velocityY   = 0.0f;
        pc.isOnGround  = true;
        pc.canJump     = true;
        if (playerEntity.hasComponent<RigidBodyComponent>())
          playerEntity.getComponent<RigidBodyComponent>().velocity.y = 0.0f;
      } else {
        // Player's top hit the bottom surface — bonked ceiling
        tf.position.y += overlapTop;
        pc.velocityY   = 0.0f;
        if (playerEntity.hasComponent<RigidBodyComponent>())
          playerEntity.getComponent<RigidBodyComponent>().velocity.y = 0.0f;
      }
    } else {
      // Lateral collision — push out on X
      if (overlapRight < overlapLeft) {
        tf.position.x -= overlapRight;
      } else {
        tf.position.x += overlapLeft;
      }
      if (playerEntity.hasComponent<RigidBodyComponent>())
        playerEntity.getComponent<RigidBodyComponent>().velocity.x = 0.0f;
    }
  }
};

#endif  // BOX_COLLISION_SYSTEM_HPP
