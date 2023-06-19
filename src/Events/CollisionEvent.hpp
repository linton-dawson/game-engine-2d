#ifndef COLLISIONEVENT_HPP
#define COLLISIONEVENT_HPP

#include "../ECS/ECS.hpp"
#include "../EventBus/Event.hpp"
class CollisionEvent: public Event {
    public:
        Entity a,b;
        CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};

#endif
