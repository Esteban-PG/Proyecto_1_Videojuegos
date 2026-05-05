#ifndef TAG_SYSTEM_HPP
#define TAG_SYSTEM_HPP

#include <string>
#include <unordered_map>

#include "../ECS/ECS.hpp"
#include "../components/tag_component.hpp"

/**
 * @brief Maintains a name→Entity lookup table for tagged entities.
 *
 * Call update() once per frame after registry->update() so the map reflects
 * the current entity set. Systems and game.cpp can then call getEntityByTag()
 * to find specific entities (e.g. "player") without iterating all entities.
 */
class TagSystem : public System {
 public:
  TagSystem() {
    requireComponent<TagComponent>();
  }

  /**
   * @brief Rebuild the tag→entity map from the current entity set.
   * Must be called once per frame before any getEntityByTag() calls.
   */
  void update() {
    taggedEntities.clear();
    for (Entity entity : getEntities()) {
      const auto& tag = entity.getComponent<TagComponent>();
      taggedEntities[tag.tag] = entity;
    }
  }

  /**
   * @brief Retrieve an entity by its tag.
   * @param tag  The tag string to look up
   * @return     The matching Entity, or Entity(-1) if not found
   */
  Entity getEntityByTag(const std::string& tag) const {
    auto it = taggedEntities.find(tag);
    if (it != taggedEntities.end()) return it->second;
    return Entity(-1);
  }

 private:
  std::unordered_map<std::string, Entity> taggedEntities;
};

#endif  // TAG_SYSTEM_HPP
