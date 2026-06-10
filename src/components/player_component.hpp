#ifndef PLAYER_COMPONENT_HPP
#define PLAYER_COMPONENT_HPP

struct PlayerComponent {
  float baseSpeed;   ///< Horizontal speed in pixels/second
  float jumpForce;   ///< Vertical impulse on jump (negative = upward)
  float gravity;     ///< Gravitational acceleration in pixels/second²
  float velocityY;   ///< Accumulated vertical velocity (integrated each frame)
  bool  isOnGround;  ///< True when the player landed on a surface this frame
  bool  canJump;     ///< True when a jump is allowed (isOnGround or coyote time)
  float baseScaleX;
  float baseScaleY;
  float spawnX;
  float spawnY;
  bool  isDead;
  bool  hasWon;
  int   score;

  PlayerComponent(float speed   = 220.0f,
                  float bScaleX = 1.0f,
                  float bScaleY = 1.0f)
      : baseSpeed(speed),
        jumpForce(-480.0f),
        gravity(900.0f),
        velocityY(0.0f),
        isOnGround(false),
        canJump(false),
        baseScaleX(bScaleX),
        baseScaleY(bScaleY),
        spawnX(0.0f),
        spawnY(0.0f),
        isDead(false),
        hasWon(false),
        score(0) {}
};

#endif  // PLAYER_COMPONENT_HPP
