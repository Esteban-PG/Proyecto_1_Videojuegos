#include "ECS.hpp"

#include <algorithm>

int IComponent::nextId = 0;

int Entity::getId() const { return id; }

void Entity::kill() { registry->destroyEntity(*this); }

void System::addEntity(Entity entity) { entities.push_back(entity); }

void System::removeEntity(Entity entity) {
  auto it = std::remove_if(entities.begin(), entities.end(),
                           [&entity](Entity other) { return entity == other; });
  entities.erase(it, entities.end());
}

std::vector<Entity> System::getEntities() const { return entities; }

const Signature& System::getComponentSignature() const {
  return componentSignature;
}

Registry::Registry() {
  std::cout << "[REGISTRY] Constructor called" << std::endl;
}

Registry::~Registry() {
  std::cout << "[REGISTRY] Destructor called" << std::endl;
}

Entity Registry::createEntity() {
  int entityId;

  if (freeEntityIds.empty()) {
    entityId = numEntity++;
    if (static_cast<long unsigned int>(entityId) >=
        entityComponentSignatures.size()) {
      entityComponentSignatures.resize(entityId + 100);
    }
  } else {
    entityId = freeEntityIds.front();
    freeEntityIds.pop_front();
  }

  Entity entity(entityId);
  entity.registry = this;
  entitiesToBeAdded.insert(entity);
  return entity;
}

void Registry::destroyEntity(Entity entity) {
  entitiesToBeRemoved.insert(entity);
}

void Registry::addEntityToSystems(Entity entity) {
  const int entityId = entity.getId();

  const Signature& entityComponentSignature =
      entityComponentSignatures[entityId];

  for (auto& system : systems) {
    const auto& systemComponentSignature =
        system.second->getComponentSignature();

    bool isInterested = (entityComponentSignature & systemComponentSignature) ==
                        systemComponentSignature;

    if (isInterested) {
      system.second->addEntity(entity);
    }
  }
}

void Registry::removeEntityFromSystems(Entity entity) {
  for (auto& system : systems) {
    system.second->removeEntity(entity);
  }
}

void Registry::update() {
  for (auto entity : entitiesToBeAdded) {
    addEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();

  for (auto entity : entitiesToBeRemoved) {
    removeEntityFromSystems(entity);

    entityComponentSignatures[entity.getId()].reset();

    freeEntityIds.push_back(entity.getId());
  }
  entitiesToBeRemoved.clear();
}

void Registry::clearAllEntities() {
  for (int i = 0; i < numEntity; i++) {
    if (entityComponentSignatures[i].any()) {
      Entity entity(i);
      removeEntityFromSystems(entity);
      entityComponentSignatures[i].reset();
    }
  }

  entitiesToBeAdded.clear();
  entitiesToBeRemoved.clear();
  numEntity = 0;
  freeEntityIds.clear();
}