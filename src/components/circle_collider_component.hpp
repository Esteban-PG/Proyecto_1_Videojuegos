#ifndef CIRCLE_COLLIDER_COMPONENT_HPP
#define CIRCLE_COLLIDER_COMPONENT_HPP

/**
 * @brief Circle collider used by CollisionSystem for circular overlap tests.
 *
 * The collision center is computed as:
 *   center = transform.position - (width/2 * scale, height/2 * scale)
 * so width/height must be set to the unscaled sprite dimensions to align
 * the circle with the visual.
 */
struct CircleColliderComponent {
  float radius; ///< Circle radius in unscaled pixels
  int   width;  ///< Sprite width used to offset the collision center
  int   height; ///< Sprite height used to offset the collision center

  CircleColliderComponent(float radius = 0.0f, int width = 0, int height = 0)
      : radius(radius), width(width), height(height) {}
};

#endif  // CIRCLE_COLLIDER_COMPONENT_HPP