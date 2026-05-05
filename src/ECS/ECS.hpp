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

struct IComponent {
 protected:
  static int nextId;
};
template <typename TComponent>
class Component : public IComponent {
  // Component data and methods
 public:
  static int getId() {
    static int id = nextId++;
    return id;
  }
};

class Entity {
 private:
  int id;

 public:
  Entity() : id(-1) {}        ///< Default-constructs an invalid entity (id=-1)
  Entity(int id) : id(id) {}
  int getId() const;
  void kill();

  bool operator==(const Entity& other) const { return id == other.id; }

  bool operator!=(const Entity& other) const { return id != other.id; }

  bool operator<(const Entity& other) const { return id < other.id; }

  bool operator>(const Entity& other) const { return id > other.id; }

  template <typename TComponent, typename... TArgs>
  void addComponent(TArgs&&... args);

  template <typename TComponent>
  void removeComponent();

  template <typename TComponent>
  bool hasComponent() const;

  template <typename TComponent>
  TComponent& getComponent() const;

  class Registry* registry;
};

class System {
 private:
  Signature componentSignature;
  std::vector<Entity> entities;

 public:
  System() = default;
  ~System() = default;

  void addEntity(Entity entity);
  void removeEntity(Entity entity);
  std::vector<Entity> getEntities() const;
  const Signature& getComponentSignature() const;

  template <typename TComponent>
  void requireComponent();
};

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

  void update();

  // Entity management
  Entity createEntity();
  void destroyEntity(Entity entity);

  // Component management
  template <typename TComponent, typename... TArgs>
  void addComponent(Entity entity, TArgs&&... args);

  template <typename TComponent>
  void removeComponent(Entity entity);

  template <typename TComponent>
  bool hasComponent(Entity entity) const;

  template <typename TComponent>
  TComponent& getComponent(Entity entity) const;

  // System management
  template <typename TSystem, typename... TArgs>
  void addSystem(TArgs&&... args);

  template <typename TSystem>
  void removeSystem();

  template <typename TSystem>
  bool hasSystem() const;

  template <typename TSystem>
  TSystem& getSystem() const;

  // Add and remove entities from systems
  void addEntityToSystems(Entity entity);
  void removeEntityFromSystems(Entity entity);

  // Reset the registry (remove all entities and components)
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

  std::cout << "[REGISTRY] Added component with ID: " << componentId
            << " to entity with ID: " << entityId << std::endl;
}

template <typename TComponent>
void Registry::removeComponent(Entity entity) {
  const int componentId = Component<TComponent>::getId();
  const int entityId = entity.getId();

  entityComponentSignatures[entityId].set(componentId, false);
  std::cout << "[REGISTRY] Removed component with ID: " << componentId
            << " from entity with ID: " << entityId << std::endl;
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