#ifndef TAG_COMPONENT_HPP
#define TAG_COMPONENT_HPP

#include <string>

/**
 * @brief Identifies an entity by a string label.
 *
 * Used to differentiate entity roles (player, wall, saw, gap, ramp, checkpoint)
 * without coupling systems to concrete component types.
 */
struct TagComponent {
  std::string tag; ///< Entity role label

  explicit TagComponent(const std::string& tag = "") : tag(tag) {}
};

#endif  // TAG_COMPONENT_HPP
