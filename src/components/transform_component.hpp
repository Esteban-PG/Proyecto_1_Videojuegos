#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm/glm.hpp>

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;

  TransformComponent(glm::vec2 position = glm::vec2(0.0f),
                     glm::vec2 scale = glm::vec2(1.0f), double rotation = 0.0)
      : position(position), scale(scale), rotation(rotation) {}
};
#endif  // TRANSFORM_COMPONENT_HPP