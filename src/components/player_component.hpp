#ifndef PLAYER_COMPONENT_HPP
#define PLAYER_COMPONENT_HPP

/**
 * @brief Core state for the player car entity in Nitro Rush.
 *
 * Jump visual effect:
 *   CarMovementSystem animates TransformComponent.scale using a sine curve
 *   relative to (baseScaleX, baseScaleY) so the effect works correctly
 *   regardless of the entity's resting scale (e.g. 0.55 for the car sprite).
 *   Scale peaks at jumpMaxScale × base at the midpoint, returns to base on
 *   landing.
 */
struct PlayerComponent {
  float baseSpeed;      ///< Normal forward speed in pixels/second
  float rotationSpeed;  ///< Turning speed in degrees/second
  bool  isJumping;      ///< True while airborne (immune to gap damage)
  float jumpTimer;      ///< Seconds remaining in the current jump
  float jumpDuration;   ///< Total jump duration in seconds
  float jumpMaxScale;   ///< Peak scale multiplier at the apex (relative to base)
  float baseScaleX;     ///< Resting X scale of the entity (restored on landing)
  float baseScaleY;     ///< Resting Y scale of the entity (restored on landing)
  int   score;          ///< Accumulated score
  bool  isDead;         ///< Set true on lethal collision
  bool  hasWon;         ///< Set true on checkpoint contact
  bool  isFalling;      ///< True while playing the gap-fall shrink animation
  float fallTimer;      ///< Seconds remaining in the fall animation
  float fallDuration;   ///< Total duration of the fall animation
  float fallStartX;     ///< Car visual-center X when fall began
  float fallStartY;     ///< Car visual-center Y when fall began
  float fallTargetX;    ///< Gap center X (destination)
  float fallTargetY;    ///< Gap center Y (destination)
  float fallHalfW;      ///< Half scaled width at fall start (for position → top-left)
  float fallHalfH;      ///< Half scaled height at fall start

  PlayerComponent(float speed    = 290.0f,
                  float rotSpeed = 130.0f,
                  float jumpDur  = 1.7f,
                  float maxScale = 1.7f,
                  float bScaleX  = 1.0f,
                  float bScaleY  = 1.0f)
      : baseSpeed(speed),
        rotationSpeed(rotSpeed),
        isJumping(false),
        jumpTimer(0.0f),
        jumpDuration(jumpDur),
        jumpMaxScale(maxScale),
        baseScaleX(bScaleX),
        baseScaleY(bScaleY),
        score(0),
        isDead(false),
        hasWon(false),
        isFalling(false),
        fallTimer(0.0f),
        fallDuration(0.5f),
        fallStartX(0.0f),
        fallStartY(0.0f),
        fallTargetX(0.0f),
        fallTargetY(0.0f),
        fallHalfW(0.0f),
        fallHalfH(0.0f) {}
};

#endif  // PLAYER_COMPONENT_HPP
