#ifndef PROJECTILEEMITSYSTEM_HPP
#define PROJECTILEEMITSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/ProjectileComponent.hpp"
#include "../Events/KeypressedEvent.hpp"
#include "../EventBus/EventBus.hpp"
#include <memory>

class ProjectileEmitSystem : public System{
    public:
        ProjectileEmitSystem() {
            requireComponent<ProjectileEmitterComponent>();
            requireComponent<TransformComponent>();
        }
        
        void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->subscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPressed);
        }

        void onKeyPressed(KeyPressedEvent& event) {
            if(event.symbol == SDLK_SPACE) {
                Logger::Log("SPACE PRESSED");
                for(auto entity : getSystemEntities()) {
                    if(entity.hasComponent<CameraFollowComponent>()) {
                        const auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
                        const auto transform = entity.getComponent<TransformComponent>();
                        const auto rigidBody = entity.getComponent<RigidBodyComponent>();

                        glm::vec2 projectilePosition = transform.position;
                        if(entity.hasComponent<SpriteComponent>()) {
                            auto sprite = entity.getComponent<SpriteComponent>();
                            projectilePosition.x += (transform.scale.x * sprite.w /2);
                            projectilePosition.y += (transform.scale.y * sprite.h /2);
                        }

                        glm::vec2 projectileVelocity = projectileEmitter.velocity;
                        int directionX = 0, directionY = 0;
                        if(rigidBody.velocity.x > 0) directionX = 1;
                        if(rigidBody.velocity.x < 0) directionX = -1;
                        if(rigidBody.velocity.y > 0) directionY = 1;
                        if(rigidBody.velocity.y < 0) directionY = -1;
                        projectileVelocity.x = projectileEmitter.velocity.x * directionX;
                        projectileVelocity.y = projectileEmitter.velocity.y * directionY;

                        Entity projectile = entity.registry->createEntity();
                        projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0,1.0), 0.0);
                        projectile.addComponent<RigidBodyComponent>(projectileVelocity);
                        projectile.addComponent<SpriteComponent>("bullet", 4, 4);
                        projectile.addComponent<BoxColliderComponent>(4, 4);
                        projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);

                    }
                }
            }
        }
        void update(std::unique_ptr<Registry>& registry) {
            for(auto entity: getSystemEntities()) {
                const auto transform = entity.getComponent<TransformComponent>();
                auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();

                if(!projectileEmitter.frequency)
                    continue;

                // check if it's time to emit
                if (SDL_GetTicks() - projectileEmitter.lastEmitTime > projectileEmitter.frequency) {
                    glm::vec2 projectilePosition = transform.position;
                    if(entity.hasComponent<SpriteComponent>()) {
                        const auto sprite = entity.getComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.w /2);
                        projectilePosition.y += (transform.scale.y * sprite.h /2);
                    }
                    // adding new projectile entity to registry
                    Entity projectile = registry->createEntity();
                    projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0,1.0), 0.0);
                    projectile.addComponent<RigidBodyComponent>(projectileEmitter.velocity);
                    projectile.addComponent<SpriteComponent>("bullet", 4, 4);
                    projectile.addComponent<BoxColliderComponent>(4, 4);
                    projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);

                    projectileEmitter.lastEmitTime = SDL_GetTicks();

                }
            }
        }
};

#endif
