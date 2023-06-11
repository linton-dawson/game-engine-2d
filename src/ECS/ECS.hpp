#ifndef ECS_HPP
#define ECS_HPP

#include <memory>
#include <bitset>
#include <cstdlib>
#include <vector>
#include <set>
#include <deque>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "../Logger/Logger.hpp"

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

class Entity { 
    private:
        int id;

    public:
        Entity(int id): id{id} {};
        Entity(const Entity& entity) = default;
        int getId() const;
        void kill();
        Entity& operator =(const Entity& other) = default;

        bool operator ==(const Entity& other) const { return id == other.id; }
        bool operator !=(const Entity& other) const { return id != other.id; }
        bool operator >(const Entity& other) const { return id > other.id; }
        bool operator <(const Entity& other) const { return id < other.id; }

        template <typename TComponent, typename ...TArgs> void addComponent(TArgs&& ...args);
        template<typename TComponent> void removeComponent();
        template<typename TComponent> bool hasComponent() const;
        template<typename TComponent> TComponent& getComponent() const;

        class Registry* registry;

};

//assign unique id to a component type
template<typename T>
class Component: public IComponent{
    public:
        static int getId() {
            static auto id = nextId++;
            return id;
        }
};

//System processes entitites with the specified signature
class System{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;
        void addEntity(Entity entity);
        void removeEntity(Entity entity);
        std::vector<Entity> getSystemEntities() const;
        const Signature& getComponentSignature() const;

        //generic function to define component types
        //required by entities to be considered by the system
        template <typename TComponent> void requireComponent();
};

class IPool {
    public:
        ~IPool(){};
};
// A pool is just a vector of object type T
template<typename TComponent>
class Pool : public IPool {
    private:
        std::vector<TComponent> data;
    public:
        Pool(int size = 100) { data.resize(size); };
        ~Pool() = default;
        bool isEmpty() { return data.empty(); }
        int getSize() {return data.size();}
        void resize(int newSize) {data.resize(newSize);}
        void set(int index, TComponent obj) { data[index] = obj;}
        void clear() {data.clear();}
        void add(TComponent object) {data.push_back(object);}
        TComponent& get(int index) {return static_cast<TComponent&>(data[index]);}
        TComponent& operator [](unsigned int index) {return data[index];}
};
class Registry{
    private:
        int numEntities = 0;
        //vector of component pools. Each pool
        //contains all data for a certain component type
        //vector index == component type id
        //pool index == entity
        std::vector<std::shared_ptr<IPool>> componentPools;

        //vector of component signatures per entity 
        //stores which component is enabled for which entity
        // vector index == entity id
        std::vector<Signature> ecSignatures;
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
        std::set<Entity> addEntityBuffer, removeEntityBuffer;

        // list of free entityids that were removed, which can be reused
        std::deque<int> freeIds;
    public: 
        Registry() = default;
        Entity createEntity();
        void update();
        void killEntity(Entity entity);
        void addEntityToSystem(Entity entity);
        void removeEntityFromSystem(Entity entity);
        template <typename TComponent, typename ...TArgs> void addComponent(Entity entity, TArgs&& ...args);
        template<typename TComponent> void removeComponent(Entity entity);
        template<typename TComponent> bool hasComponent(Entity entity) const;
        template<typename TComponent> TComponent& getComponent(Entity entity) const;

        //System management
        template<typename TSystem, typename ...TArgs> void addSystem(TArgs&& ...args);
        template<typename TSystem> void removeSystem();
        template<typename TSystem> bool hasSystem() const;
        template<typename TSystem> TSystem& getSystem() const;

};

template<typename TSystem, typename ...TArgs>
void Registry::addSystem(TArgs&& ...args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)),newSystem));
}

template<typename TSystem>
void Registry::removeSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem>
bool Registry::hasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& Registry::getSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent>
void System::requireComponent() {
    const auto componentId = Component<TComponent>::getId();
    componentSignature.set(componentId);
}

template<typename TComponent, typename ...TArgs>
void Registry::addComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    if(componentId >= static_cast<int>(componentPools.size())) {
        componentPools.resize(componentId + 1, nullptr);
    }
    if(!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    if(entityId >= componentPool->getSize()) {
        componentPool->resize(numEntities);
    }
    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->set(entityId, newComponent);

    //set the bit at componentId^th position
    ecSignatures[entityId].set(componentId);
    Logger::Log("Component id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));
}

template<typename TComponent>
void Registry::removeComponent(Entity entity) {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    ecSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::hasComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    return ecSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent& Registry::getComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::getId();
    const auto entityId = entity.getId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->get(entityId);
}

//should be a pointer to "this" for all Registry wrapper methods ?
template <typename TComponent, typename ...TArgs> 
void Entity::addComponent(TArgs&& ...args) {
    registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}
template<typename TComponent> 
void Entity::removeComponent() {
    registry->removeComponent<TComponent>(*this);
}
template<typename TComponent> 
bool Entity::hasComponent() const {
    return registry->hasComponent<TComponent>(*this);
}
template<typename TComponent> 
TComponent& Entity::getComponent() const {
    return registry->getComponent<TComponent>(*this);
}
#endif
