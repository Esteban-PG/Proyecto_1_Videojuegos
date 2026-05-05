#ifndef NITRO_COMPONENT_HPP
#define NITRO_COMPONENT_HPP

/**
 * @brief Manages the nitro boost state and cooldown for the player car.
 *
 * Nitro activates on a key PRESS (edge trigger, not held-state).
 * After activation it boosts speed for `boostDuration` seconds, then enters a
 * cooldown of `cooldownTime` seconds.  Holding the key does NOT re-trigger
 * nitro once the boost expires — the player must release and press again.
 *
 * `keyWasHeld` stores whether the nitro key was pressed last frame so
 * NitroSystem can detect the rising edge (false → true) and ignore held state.
 */
struct NitroComponent {
  float cooldownTime;       ///< Full cooldown between uses (seconds)
  float cooldownRemaining;  ///< Seconds left on the current cooldown
  float boostDuration;      ///< How long each activation lasts (seconds)
  float boostRemaining;     ///< Seconds of boost remaining this use
  bool  isOnCooldown;       ///< True while cooldown is counting down
  bool  isBoosting;         ///< True while speed boost is active
  bool  keyWasHeld;         ///< Nitro key state from the PREVIOUS frame (edge detect)

  NitroComponent(float cooldown = 1.0f, float boost = 1.0f)
      : cooldownTime(cooldown),
        cooldownRemaining(0.0f),
        boostDuration(boost),
        boostRemaining(0.0f),
        isOnCooldown(false),
        isBoosting(false),
        keyWasHeld(false) {}
};

#endif  // NITRO_COMPONENT_HPP
