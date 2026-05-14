#ifndef BOX_COLLISION_SYSTEM_HPP
#define BOX_COLLISION_SYSTEM_HPP

#include <algorithm>
#include <string>

#include "../ECS/ECS.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/nitro_component.hpp"
#include "../components/player_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/tag_component.hpp"
#include "../components/transform_component.hpp"
#include "../game/game.hpp"
#include "../systems/tag_system.hpp"

/**
 * @brief AABB collision system for world-geometry interactions.
 *
 * Every frame it locates the player entity (via TagSystem) and checks its
 * BoxColliderComponent against every other BoxCollider entity.
 * Responses per tag:
 *   - "wall"       → push player out on the minimum-overlap axis and damp
 *                    the perpendicular velocity component (bounce).
 *   - "gap"        → instant death unless player.isJumping is true.
 *   - "ramp"       → triggers a jump if nitro was used within the last 1 second.
 *   - "checkpoint" → sets player.hasWon = true.
 *
 * Saws (circle colliders) are still handled by CollisionSystem + DamageSystem.
 */
class BoxCollisionSystem : public System {
 public:
  BoxCollisionSystem() {
    requireComponent<BoxColliderComponent>();
    requireComponent<TransformComponent>();
    requireComponent<TagComponent>();
  }

  /**
   * @brief Run one collision pass for the player against all AABB objects.
   */
  void update() {
    auto& tagSystem = Game::getInstance().registry->getSystem<TagSystem>();
    Entity playerEntity = tagSystem.getEntityByTag("player");
    if (playerEntity.getId() < 0) return;
    if (!playerEntity.hasComponent<PlayerComponent>()) return;

    auto& playerTransform = playerEntity.getComponent<TransformComponent>();
    auto& playerBox       = playerEntity.getComponent<BoxColliderComponent>();
    auto& playerComp      = playerEntity.getComponent<PlayerComponent>();

    if (playerComp.isDead || playerComp.hasWon) return;

    const float pw = playerBox.width  * playerTransform.scale.x;
    const float ph = playerBox.height * playerTransform.scale.y;
    const float px = playerTransform.position.x;
    const float py = playerTransform.position.y;

    for (Entity entity : getEntities()) {
      if (entity == playerEntity) continue;

      const auto& tag       = entity.getComponent<TagComponent>();
      const auto& transform = entity.getComponent<TransformComponent>();
      const auto& box       = entity.getComponent<BoxColliderComponent>();

      const float ew = box.width  * transform.scale.x;
      const float eh = box.height * transform.scale.y;
      const float ex = transform.position.x;
      const float ey = transform.position.y;

      if (!overlaps(px, py, pw, ph, ex, ey, ew, eh)) continue;

      handleCollision(tag.tag, entity, playerEntity,
                      playerTransform, playerComp,
                      px, py, pw, ph,
                      ex, ey, ew, eh);
    }
  }

 private:
  static bool overlaps(float ax, float ay, float aw, float ah,
                        float bx, float by, float bw, float bh) {
    return ax < bx + bw && ax + aw > bx &&
           ay < by + bh && ay + ah > by;
  }

  void handleCollision(const std::string& tag, Entity /*worldEntity*/,
                        Entity playerEntity,
                        TransformComponent& tf, PlayerComponent& pc,
                        float px, float py, float pw, float ph,
                        float ex, float ey, float ew, float eh) {
    if (tag == "wall") {
      resolveWallBounce(playerEntity, tf, pc, px, py, pw, ph, ex, ey, ew, eh);
    } else if (tag == "gap") {
      if (!pc.isJumping && !pc.isFalling && !pc.isDead) {
        pc.isFalling   = true;
        pc.fallTimer   = pc.fallDuration;
        pc.fallHalfW   = pw * 0.5f;
        pc.fallHalfH   = ph * 0.5f;
        pc.fallStartX  = px + pc.fallHalfW;
        pc.fallStartY  = py + pc.fallHalfH;
        pc.fallTargetX = ex + ew * 0.5f;
        pc.fallTargetY = ey + eh * 0.5f;
        Game::getInstance().audioManager->playSFX("death");
      }
    } else if (tag == "ramp") {
      if (!pc.isJumping && playerEntity.hasComponent<NitroComponent>()) {
        const auto& nitro = playerEntity.getComponent<NitroComponent>();
        if (nitro.isBoosting) {
          pc.isJumping = true;
          pc.jumpTimer = pc.jumpDuration;
          pc.score    += 75;
          Game::getInstance().audioManager->playSFX("jump");
          Game::getInstance().triggerShake(0.08f, 5.0f);
        }
      }
    } else if (tag == "checkpoint") {
      if (!pc.hasWon) {
        pc.hasWon = true;
        Game::getInstance().audioManager->playSFX("win");
      }
    } else if (tag == "saw") {
      if (!pc.isDead && !pc.isFalling) {
        pc.isDead = true;
        Game::getInstance().audioManager->playSFX("death");
        Game::getInstance().triggerShake(0.25f, 7.0f);
      }
    }
  }

  void resolveWallBounce(Entity playerEntity,
                          TransformComponent& tf, PlayerComponent& /*pc*/,
                          float px, float py, float pw, float ph,
                          float ex, float ey, float ew, float eh) {
    // Overlap amounts on each axis
    const float overlapRight  = (px + pw) - ex;
    const float overlapLeft   = (ex + ew) - px;
    const float overlapBottom = (py + ph) - ey;
    const float overlapTop    = (ey + eh) - py;

    const float minX = std::min(overlapRight, overlapLeft);
    const float minY = std::min(overlapBottom, overlapTop);

    if (!playerEntity.hasComponent<RigidBodyComponent>()) {
      // No velocity — just push out
      if (minX < minY) {
        tf.position.x += (overlapRight < overlapLeft) ? -overlapRight : overlapLeft;
      } else {
        tf.position.y += (overlapBottom < overlapTop) ? -overlapBottom : overlapTop;
      }
      return;
    }

    auto& body = playerEntity.getComponent<RigidBodyComponent>();

    if (minX < minY) {
      // Resolve on X axis — reflect X velocity
      if (overlapRight < overlapLeft) {
        tf.position.x -= overlapRight;
      } else {
        tf.position.x += overlapLeft;
      }
      body.velocity.x *= -0.4f;
      Game::getInstance().audioManager->playSFX("bounce");
    } else {
      // Resolve on Y axis — reflect Y velocity
      if (overlapBottom < overlapTop) {
        tf.position.y -= overlapBottom;
      } else {
        tf.position.y += overlapTop;
      }
      body.velocity.y *= -0.4f;
      Game::getInstance().audioManager->playSFX("bounce");
    }
  }
};

#endif  // BOX_COLLISION_SYSTEM_HPP
