#ifndef KEYBOARDMOVEMENTSYSTEM_HPP
#define KEYBOARDMOVEMENTSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/KeypressedEvent.hpp"
#include "../Components/KeyboardControlledComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"


class KeyboardMovementSystem: public System {
    public:
        KeyboardMovementSystem() {
            requireComponent<KeyboardControlledComponent>();
            requireComponent<SpriteComponent>();
            requireComponent<RigidBodyComponent>();
        }

        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::onKeyPressed);
        }

        void onKeyPressed(KeyPressedEvent& event) {
            for(auto entity: getSystemEntities()) {
                auto keyboardControl = entity.getComponent<KeyboardControlledComponent>();
                auto sprite = entity.getComponent<SpriteComponent>();
                auto rigidBody = entity.getComponent<RigidBodyComponent>();
            }
        }
};
#endif
