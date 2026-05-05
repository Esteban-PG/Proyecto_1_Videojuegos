#ifndef CIRCLE_COLLIDER_COMPONENT_HPP
#define CIRCLE_COLLIDER_COMPONENT_HPP

struct CircleColliderComponent {
  float radius;
  int width;
  int height;

  CircleColliderComponent(float radius = 0.0f, int width = 0, int height = 0)
      : radius(radius), width(width), height(height) {}
};

#endif  // CIRCLE_COLLIDER_COMPONENT_HPP