#ifndef DAMAGESYSTEM_HPP
#define DAMAGESYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/CollisionEvent.hpp"

class DamageSystem: public System {
    public:
        DamageSystem() {
            requireComponent<BoxColliderComponent>();
        }

        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        void onCollision(CollisionEvent& event) {
            Logger::Log("DamageSystem: Collision between " + std::to_string(event.a.getId()) + " and " + std::to_string(event.b.getId()));
            event.a.kill();
            event.b.kill();
        }

        void update() {
        }
};
#endif
