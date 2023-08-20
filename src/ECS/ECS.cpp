#include "ECS.hpp"
#include "../Logger/Logger.hpp"
#include <algorithm>

//init static nextId 
int IComponent::nextId = 0;

int Entity::getId() const {
    return id;
}

void Entity::kill() {
    registry->killEntity(*this);
}

void Entity::tag(const std::string& tag) {
    registry->tagEntity(*this, tag);
}

bool Entity::hasTag(const std::string& tag) const{
    return registry->entityHasTag(*this,tag);
}

void Entity::group(const std::string& group) {
    registry->groupEntity(*this, group);
}

bool Entity::belongsToGroup(const std::string& group) const {
    return registry->entityBelongsToGroup(*this, group);
}

void System::addEntity(Entity entity) {
    entities.push_back(entity);
}

void System::removeEntity(Entity entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&entity](Entity other){return entity == other;})
                ,entities.end());
}

std::vector<Entity> System::getSystemEntities() const {
    return entities;
}

const Signature& System::getComponentSignature() const {
    return componentSignature;
}

Entity Registry::createEntity() {
    int entityId;
    if(freeIds.empty()) {
        entityId = numEntities++;
        if(entityId >= static_cast<int>(ecSignatures.size())) {
        ecSignatures.resize(entityId + 1);
        }
    }
    else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }
    Entity entity(entityId);
    entity.registry = this;
    Logger::Log("Inserting entityId = " + std::to_string(entityId));
    addEntityBuffer.insert(entity);
    
    Logger::Log("Entity created with id " + std::to_string(entityId));
    return entity;
}

void Registry::killEntity(Entity entity) {
    removeEntityBuffer.insert(entity);
}

void Registry::addEntityToSystem(Entity entity) {
    const auto entityId = entity.getId();
    const auto& ecsId = ecSignatures[entityId];
    for(auto& system : systems) {
        const auto& sysCompoSign = system.second->getComponentSignature();
        if((ecsId & sysCompoSign) == sysCompoSign) {
            system.second->addEntity(entity);
        }
    }
}

void Registry::removeEntityFromSystem(Entity entity) {
    for(auto system: systems) {
        system.second->removeEntity(entity);
    }
}

void Registry::tagEntity(Entity entity, const std::string& tag) {
    entityPerTag.emplace(tag, entity);
}

bool Registry::entityHasTag(Entity entity, const std::string& tag) const {
    if(tagPerEntity.find(entity.getId()) == tagPerEntity.end())
        return false;
    return entityPerTag.find(tag)->second == entity;
}

Entity Registry::getEntityByTag(const std::string& tag) const {
    return entityPerTag.at(tag);
}

void Registry::removeEntityTag(Entity entity) {
    if(auto taggedEntity = tagPerEntity.find(entity.getId()); taggedEntity != tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void Registry::groupEntity(Entity entity, const std::string& group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup.at(group).emplace(entity);
    groupPerEntity.emplace(entity.getId(), group);
}

bool Registry::entityBelongsToGroup(Entity entity, const std::string& group) {
    auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.find(entity.getId()) != groupEntities.end();
}

std::vector<Entity> Registry::getEntityByGroup(const std::string& group) const {
    auto& setOfEntities = entitiesPerGroup.find(group)->second;
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::removeEntityGroup(Entity entity) {
    if(auto groupedEntity = groupPerEntity.find(entity.getId()); groupedEntity != groupPerEntity.end()) {
        if(auto group = entitiesPerGroup.find(groupedEntity->second); group != entitiesPerGroup.end()) {
            if(auto entitiyInGroup = group->second.find(entity); entitiyInGroup != group->second.end()) {
                group->second.erase(entitiyInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

void Registry::update() {
    // add queued entities to active Systems
    // TODO: Maybe improve 
    for(auto entity : addEntityBuffer) {
        addEntityToSystem(entity);
    }
    addEntityBuffer.clear();

    // remove queued entities from active systems
    for(auto entity : removeEntityBuffer) {
        removeEntityFromSystem(entity);
        freeIds.push_back(entity.getId());
        ecSignatures[entity.getId()].reset();

        // entity tag and group cleanup
        removeEntityTag(entity);
        removeEntityGroup(entity);
    }
    removeEntityBuffer.clear();
}

