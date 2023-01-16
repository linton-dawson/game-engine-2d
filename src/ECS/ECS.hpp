#ifndef ECS_HPP
#define ECS_HPP

#include <memory>
#include <bitset>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
        int getId() const;
};

//assign unique id to a component type
template<typename T>
class Component: public IComponent{
    static int getId() {
        return nextId++;
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
        virtual ~IPool(){}
};

// A pool is just a vector of object type T
template<typename T>
class Pool : public IPool{
    private:
        std::vector<T> data;
    public:
        Pool(int size = 100) { data.resize(size); };
        virtual ~Pool() = default;
        bool isEmpty() { return data.empty(); }
        int getSize() {return data.size();}
        void resize(int newSize) {data.resize(newSize);}
        void clear() {data.clear();}
        void add(T obj) {data.emplace_back(obj);}
        void set(int index, T obj) {data[index] = obj;}
        void get(int index) {return static_cast<T>(data[index]);}
        T& operator[](unsigned int index) {return data[index];}
};

// Registry manages lifespan of entities, can add systems and components (aka World class)
class Registry{
    private:
        int numEntities = 0;

        //vector of component pools. Each pool
        //contains all data for a certain component type
        //vector index == component type id
        //pool index == entity
        std::vector<std::shared_ptr<IPool>> componentPools;

        //component signatures to keep track of signatures enabled for an entity
        std::vector<Signature> entityComponentSignature;

        //mapping of type index to system
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    public:
        Registry() = default;
               
};

template<typename TComponent>
void System::requireComponent() {
    const auto componentId = Component<TComponent>::getId();
    componentSignature.set(componentId);
}
#endif
