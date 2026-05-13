#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm/glm.hpp>

/**
 * @brief World-space transform for any renderable or collidable entity.
 *
 * position is the top-left corner of the entity in world pixels.
 * scale multiplies the sprite/collider dimensions at render and collision time.
 * rotation is in degrees, applied around the sprite center by SDL_RenderCopyEx.
 */
struct TransformComponent {
  glm::vec2 position; ///< Top-left world position in pixels
  glm::vec2 scale;    ///< Multiplicative scale applied to width and height
  double    rotation; ///< Rotation in degrees (clockwise, SDL convention)

  TransformComponent(glm::vec2 position = glm::vec2(0.0f),
                     glm::vec2 scale = glm::vec2(1.0f), double rotation = 0.0)
      : position(position), scale(scale), rotation(rotation) {}
};
#endif  // TRANSFORM_COMPONENT_HPP