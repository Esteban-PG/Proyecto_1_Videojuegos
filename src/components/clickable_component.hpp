#ifndef CLICKABLE_COMPONENT_HPP
#define CLICKABLE_COMPONENT_HPP

/**
 * @brief Marker component that makes an entity participate in click detection.
 *
 * UISystem checks for this component (together with TextComponent) to decide
 * whether a ClickEvent should trigger the entity's ScriptComponent::onClick.
 */
struct ClickableComponent {
  bool isClicked; ///< True during the frame the entity was clicked
  ClickableComponent() : isClicked(false) {}
};

#endif  // CLICKABLE_COMPONENT_HPP