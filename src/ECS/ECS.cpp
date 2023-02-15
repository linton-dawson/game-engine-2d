#include "ECS.hpp"
#include "../Logger/Logger.hpp"
#include <algorithm>

//init static nextId 
int IComponent::nextId = 0;

int Entity::getId() const {
    return id;
}

void System::addEntity(Entity entity) {
    entities.push_back(entity);
}

void System::removeEntity(Entity entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&entity](Entity other){return entity.getId() == other.getId();})
                ,entities.end());
}

std::vector<Entity> System::getSystemEntities() const {
    return entities;
}

const Signature& System::getComponentSignature() const {
    return componentSignature;
}

Entity Registry::createEntity() {
    int entityId = numEntities++;
    Entity entity(entityId);
    addEntityBuffer.insert(entity);
    if(entityId >= static_cast<int>(ecSignatures.size())) {
        ecSignatures.resize(entityId + 1);
    }
    Logger::Log("Entity created with id " + std::to_string(entityId));
    return entity;
}

void Registry::killEntity(Entity entity) {
    removeEntityBuffer.insert(entity);
}

void Registry::addEntityToSystem(Entity entity) {
    const auto entityId = entity.getId();
    for(auto& system : systems) {
        const auto& sysCompoSign = system.second->getComponentSignature();
        if((ecSignatures[entityId] ^ sysCompoSign) == 0) {
            system.second->addEntity(entity);
        }
    }
}
void Registry::update() {
    // add queued entities to active Systems
    // TODO: Maybe improve 
    for(auto entity : addEntityBuffer) {
        addEntityToSystem(entity);
    }
    addEntityBuffer.clear();
}

