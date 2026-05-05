#ifndef NITRO_SYSTEM_HPP
#define NITRO_SYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../components/nitro_component.hpp"
#include "../components/player_component.hpp"
#include "../game/game.hpp"

/**
 * @brief Manages the nitro boost/cooldown lifecycle for the player car.
 *
 * Activation uses EDGE DETECTION: nitro fires only on the frame the key goes
 * from not-pressed to pressed (rising edge).  Holding the key does nothing
 * once the boost is already running or the cooldown has just expired — the
 * player must release and press again to get another burst.
 *
 * Each frame:
 * 1. Counts down the active boost timer; clears isBoosting when it expires.
 * 2. Counts down the cooldown timer; clears isOnCooldown when it expires.
 * 3. Samples the "nitro" action and compares to the previous-frame state.
 *    Only a false→true transition triggers a new activation.
 * 4. Updates keyWasHeld for the next frame's comparison.
 */
class NitroSystem : public System {
 public:
  NitroSystem() {
    requireComponent<NitroComponent>();
    requireComponent<PlayerComponent>();
  }

  /**
   * @brief Tick nitro state and detect a fresh key press.
   * @param deltaTime Seconds elapsed since last frame
   */
  void update(double deltaTime) {
    const float dt = static_cast<float>(deltaTime);
    auto& ctrl = *Game::getInstance().controllerManager;

    for (Entity entity : getEntities()) {
      auto& nitro  = entity.getComponent<NitroComponent>();
      auto& player = entity.getComponent<PlayerComponent>();

      if (player.isDead || player.hasWon) continue;

      // ── Count down boost ──────────────────────────────────────────────────
      if (nitro.isBoosting) {
        nitro.boostRemaining -= dt;
        if (nitro.boostRemaining <= 0.0f) {
          nitro.isBoosting     = false;
          nitro.boostRemaining = 0.0f;
        }
      }

      // ── Count down cooldown ───────────────────────────────────────────────
      if (nitro.isOnCooldown) {
        nitro.cooldownRemaining -= dt;
        if (nitro.cooldownRemaining <= 0.0f) {
          nitro.isOnCooldown      = false;
          nitro.cooldownRemaining = 0.0f;
        }
      }

      // ── Edge detection: only fire on the rising edge of the key ──────────
      const bool keyNow     = ctrl.isActionActivated("nitro");
      const bool justPressed = keyNow && !nitro.keyWasHeld; // false→true
      nitro.keyWasHeld = keyNow;                             // store for next frame

      if (justPressed && !nitro.isOnCooldown) {
        nitro.isBoosting        = true;
        nitro.boostRemaining    = nitro.boostDuration;
        nitro.isOnCooldown      = true;
        nitro.cooldownRemaining = nitro.cooldownTime;

        Game::getInstance().audioManager->playSFX("nitro");
      }
    }
  }
};

#endif  // NITRO_SYSTEM_HPP
