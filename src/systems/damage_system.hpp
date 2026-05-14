#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

#include <iostream>
#include <memory>

#include "../ECS/ECS.hpp"
#include "../components/circle_collider_component.hpp"
#include "../components/player_component.hpp"
#include "../components/tag_component.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/collision_event.hpp"
#include "../game/game.hpp"

/**
 * @brief Handles the outcome of circle-circle collision events.
 *
 * When a CollisionEvent fires (emitted by CollisionSystem), this system
 * inspects the tags of both entities to decide the response:
 *
 *   player + saw  → set PlayerComponent::isDead = true, play "death" SFX.
 *   any + any     → default (ignored) — AABB/gap deaths are handled by
 *                   BoxCollisionSystem instead.
 *
 * Saws themselves are NOT killed, so they remain as permanent obstacles.
 */
class DamageSystem : public System {
 public:
  DamageSystem() { requireComponent<CircleColliderComponent>(); }

  void subscribeToCollisionEvent(std::unique_ptr<EventManager>& eventManager) {
    eventManager->subscribeToEvent<CollisionEvent, DamageSystem>(
        this, &DamageSystem::onCollision);
  }

  void onCollision(CollisionEvent& event) {
    Entity a = event.entityA;
    Entity b = event.entityB;

    bool aIsPlayer = hasTag(a, "player");
    bool bIsPlayer = hasTag(b, "player");
    bool aIsSaw    = hasTag(a, "saw");
    bool bIsSaw    = hasTag(b, "saw");

    if ((aIsPlayer && bIsSaw) || (aIsSaw && bIsPlayer)) {
      Entity player = aIsPlayer ? a : b;
      if (player.hasComponent<PlayerComponent>()) {
        auto& pc = player.getComponent<PlayerComponent>();
        if (!pc.isDead) {
          std::cout << "[DAMAGE] Player hit a saw — dead." << std::endl;
          pc.isDead = true;
          Game::getInstance().audioManager->playSFX("death");
          Game::getInstance().triggerShake(0.25f, 7.0f);
        }
      } else {
        // Fallback: kill entity if no PlayerComponent (legacy behaviour)
        player.kill();
      }
      return;
    }

    // For all other pairs do nothing — extending this is straightforward.
  }

 private:
  static bool hasTag(Entity entity, const std::string& tag) {
    return entity.hasComponent<TagComponent>() &&
           entity.getComponent<TagComponent>().tag == tag;
  }
};

#endif  // DAMAGE_SYSTEM_HPP
