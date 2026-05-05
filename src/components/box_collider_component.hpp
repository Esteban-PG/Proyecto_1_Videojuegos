#ifndef BOX_COLLIDER_COMPONENT_HPP
#define BOX_COLLIDER_COMPONENT_HPP

/**
 * @brief Axis-aligned bounding box (AABB) collider.
 *
 * Position is derived from the entity's TransformComponent (top-left corner).
 * Used for wall bounce, gap detection, ramp detection, and checkpoint detection.
 */
struct BoxColliderComponent {
  float width;  ///< Collider width in world pixels
  float height; ///< Collider height in world pixels

  BoxColliderComponent(float w = 32.0f, float h = 32.0f)
      : width(w), height(h) {}
};

#endif  // BOX_COLLIDER_COMPONENT_HPP
