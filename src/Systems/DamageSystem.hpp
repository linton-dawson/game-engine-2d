#ifndef DAMAGESYSTEM_HPP
#define DAMAGESYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/ProjectileComponent.hpp"
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
            Entity a = event.a;
            Entity b = event.b;
            if(a.belongsToGroup("projectiles")) {
                Logger::Log("DamageSystem: Collision between " + std::to_string(event.a.getId()) + " and " + std::to_string(event.b.getId()));
                if(b.hasTag("player")) {
                    Logger::Log("VALID COLLISION !!!!!");
                    onProjectileHitPlayer(a,b);
                }
                if(b.belongsToGroup("enemies")) {
                    onProjectileHitEnemy(a,b);
                }
            }
            if(b.belongsToGroup("projectiles")) {
                Logger::Log("DamageSystem: Collision between " + std::to_string(event.a.getId()) + " and " + std::to_string(event.b.getId()));
                if(a.hasTag("player")) {
                    Logger::Log("VALID COLLISION !!!!!");
                    onProjectileHitPlayer(b,a);
                }
                if(a.belongsToGroup("enemies")) {
                    onProjectileHitEnemy(b,a);
                }
            }
        }

        void onProjectileHitPlayer(Entity proj, Entity player) {
            auto projComponent = proj.getComponent<ProjectileComponent>();
            if(!projComponent.isFriendly) {
                auto& health = player.getComponent<HealthComponent>();
                health.health -= projComponent.hitPercentDamage;
                Logger::Log("CURRENT PLAYER HEALTH IS " + std::to_string(health.health));
                if(health.health <= 0) {
                    player.kill();
                }
                proj.kill();
            }
        }

        void onProjectileHitEnemy(Entity proj, Entity enemy) {
            auto projComponent = proj.getComponent<ProjectileComponent>();
            if(projComponent.isFriendly) {
                auto& health = enemy.getComponent<HealthComponent>();
                health.health -= projComponent.hitPercentDamage;
                Logger::Log("CURRENT ENEMY HEALTH IS " + std::to_string(health.health));
                if(health.health <= 0) {
                    enemy.kill();
                }
                proj.kill();

            }
        }
        void update() {
        }
};
#endif
