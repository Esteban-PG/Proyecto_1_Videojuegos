#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "../utils/pool.hpp"

const unsigned int MAX_COMPONENTS = 64;

// Signature
typedef std::bitset<MAX_COMPONENTS> Signature;

/** @brief Base for the Component<T> CRTP helper — holds the shared ID counter. */
struct IComponent {
 protected:
  static int nextId;
};

/**
 * @brief CRTP helper that assigns a unique integer ID to each component type.
 * @tparam TComponent The concrete component type
 */
template <typename TComponent>
class Component : public IComponent {
 public:
  /** @brief @return Stable integer ID for TComponent, assigned on first call. */
  static int getId() {
    static int id = nextId++;
    return id;
  }
};

/**
 * @brief Lightweight handle to a game object — just an integer ID.
 *
 * Entities do not store components directly; Registry maps the ID to
 * component pools and system membership. An entity with id=-1 is invalid.
 */
class Entity {
 private:
  int id;

 public:
  Entity() : id(-1) {}        ///< Default-constructs an invalid entity (id = -1)
  explicit Entity(int id) : id(id) {}
  /** @brief @return The entity's unique integer identifier. */
  int  getId() const;
  /** @brief Schedule this entity for removal at the next Registry::update(). */
  void kill();

  bool operator==(const Entity& other) const { return id == other.id; }

  bool operator!=(const Entity& other) const { return id != other.id; }

  bool operator<(const Entity& other) const { return id < other.id; }

  bool operator>(const Entity& other) const { return id > other.id; }

  /** @brief Construct and attach a component to this entity. */
  template <typename TComponent, typename... TArgs>
  void addComponent(TArgs&&... args);
  /** @brief Detach a component from this entity. */
  template <typename TComponent>
  void removeComponent();
  /** @brief @return True if this entity has TComponent attached. */
  template <typename TComponent>
  bool hasComponent() const;
  /** @brief @return Reference to this entity's TComponent. */
  template <typename TComponent>
  TComponent& getComponent() const;

  class Registry* registry;
};

/**
 * @brief Base class for all ECS systems.
 *
 * A system declares which component types it needs via requireComponent<T>().
 * Registry automatically adds/removes entities from each system whenever their
 * component set changes so that getEntities() always returns the right set.
 */
class System {
 private:
  Signature componentSignature;   ///< Bitmask of required component IDs
  std::vector<Entity> entities;   ///< Entities currently matching this system

 public:
  System() = default;
  ~System() = default;

  /** @brief Called by Registry when a matching entity is created. */
  void addEntity(Entity entity);
  /** @brief Called by Registry when an entity no longer matches. */
  void removeEntity(Entity entity);
  /** @brief @return All entities currently tracked by this system. */
  std::vector<Entity> getEntities() const;
  /** @brief @return The component bitmask this system requires. */
  const Signature& getComponentSignature() const;

  /**
   * @brief Declare that this system requires TComponent on every entity.
   * @tparam TComponent Component type that must be present
   */
  template <typename TComponent>
  void requireComponent();
};

/**
 * @brief Central ECS registry — creates entities and manages components/systems.
 *
 * Entities are integer IDs. Components are stored in typed Pool<T> arrays indexed
 * by entity ID. Systems are matched against each entity's component Signature
 * at Registry::update() time (deferred add/remove for safe in-loop modification).
 */
class Registry {
 private:
  int numEntity = 0;
  std::vector<std::shared_ptr<IPool>> componentsPools;
  std::vector<Signature> entityComponentSignatures;

  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeRemoved;

  std::deque<int> freeEntityIds;

 public:
  Registry();
  ~Registry();

  /** @brief Flush deferred entity additions and removals into systems. */
  void update();

  /** @brief Create a new entity and return its handle. */
  Entity createEntity();
  /** @brief Schedule @p entity for removal at the next update(). */
  void destroyEntity(Entity entity);

  /** @brief Attach a component to @p entity, constructing it with @p args. */
  template <typename TComponent, typename... TArgs>
  void addComponent(Entity entity, TArgs&&... args);
  /** @brief Detach TComponent from @p entity. */
  template <typename TComponent>
  void removeComponent(Entity entity);
  /** @brief @return True if @p entity has TComponent. */
  template <typename TComponent>
  bool hasComponent(Entity entity) const;
  /** @brief @return Reference to TComponent on @p entity. */
  template <typename TComponent>
  TComponent& getComponent(Entity entity) const;

  /** @brief Instantiate and register a system of type TSystem. */
  template <typename TSystem, typename... TArgs>
  void addSystem(TArgs&&... args);
  /** @brief Remove and destroy the TSystem instance. */
  template <typename TSystem>
  void removeSystem();
  /** @brief @return True if TSystem is registered. */
  template <typename TSystem>
  bool hasSystem() const;
  /** @brief @return Reference to the registered TSystem. */
  template <typename TSystem>
  TSystem& getSystem() const;

  /** @brief Add @p entity to every system whose signature it satisfies. */
  void addEntityToSystems(Entity entity);
  /** @brief Remove @p entity from all systems. */
  void removeEntityFromSystems(Entity entity);
  /** @brief Destroy all entities and reset the registry to empty state. */
  void clearAllEntities();
};

template <typename TComponent>
void System::requireComponent() {
  const int componentId = Component<TComponent>::getId();
  componentSignature.set(componentId);
}

template <typename TComponent, typename... TArgs>
void Registry::addComponent(Entity entity, TArgs&&... args) {
  const int componentId = Component<TComponent>::getId();
  const int entityId = entity.getId();

  if (static_cast<std::size_t>(componentId) >= componentsPools.size()) {
    componentsPools.resize(componentId + 10, nullptr);
  }

  if (!componentsPools[componentId]) {
    std::shared_ptr<Pool<TComponent>> newComponentPool =
        std::make_shared<Pool<TComponent>>();
    componentsPools[componentId] = newComponentPool;
  }

  std::shared_ptr<Pool<TComponent>> componentPool =
      std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);

  if (entityId >= componentPool->getSize()) {
    componentPool->resize(numEntity + 100);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);
  componentPool->set(entityId, newComponent);
  entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::removeComponent(Entity entity) {
  const int componentId = Component<TComponent>::getId();
  const int entityId = entity.getId();

  entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::hasComponent(Entity entity) const {
  const int componentId = Component<TComponent>::getId();
  const int entityId = entity.getId();

  return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::getComponent(Entity entity) const {
  const int componentId = Component<TComponent>::getId();
  const int entityId = entity.getId();

  auto componentPool =
      std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);

  return componentPool->get(entityId);
}

template <typename TSystem, typename... TArgs>
void Registry::addSystem(TArgs&&... args) {
  std::shared_ptr<TSystem> newSystem =
      std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::removeSystem() {
  auto system = systems.find(std::type_index(typeid(TSystem)));
  systems.erase(system);
}

template <typename TSystem>
bool Registry::hasSystem() const {
  return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::getSystem() const {
  auto system = systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename... TArgs>
void Entity::addComponent(TArgs&&... args) {
  registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::removeComponent() {
  registry->removeComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::hasComponent() const {
  return registry->hasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::getComponent() const {
  return registry->getComponent<TComponent>(*this);
}

#endif  // ECS_HPP