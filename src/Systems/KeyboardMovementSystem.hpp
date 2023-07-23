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
                const auto keyboardControl = entity.getComponent<KeyboardControlledComponent>();
                auto& sprite = entity.getComponent<SpriteComponent>();
                auto& rigidBody = entity.getComponent<RigidBodyComponent>();
                switch(event.symbol) {
                    case SDLK_w:
                        rigidBody.velocity = keyboardControl.up;
                        sprite.srcRect.y = sprite.h * 0;
                        break;
                    case SDLK_d:
                        rigidBody.velocity = keyboardControl.right;
                        sprite.srcRect.y = sprite.h * 1;
                        break;
                    case SDLK_s:
                        rigidBody.velocity = keyboardControl.down;
                        sprite.srcRect.y = sprite.h * 2;
                        break;
                    case SDLK_a:
                        rigidBody.velocity = keyboardControl.left;
                        sprite.srcRect.y = sprite.h * 3;
                        break;
                }
            }
        }
};
#endif
