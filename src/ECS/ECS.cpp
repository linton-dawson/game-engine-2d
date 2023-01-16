#include "ECS.hpp"
#include <algorithm>

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

