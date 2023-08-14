#ifndef PROJECTILELIFECYCLESYSTEM_HPP
#define PROJECTILELIFECYCLESYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/ProjectileComponent.hpp"

class ProjectileLifecycleSystem : public System {
    public:
        ProjectileLifecycleSystem() {
            requireComponent<ProjectileComponent>();
        }

        void update() {
            for(auto entity: getSystemEntities()) {
                auto projectile = entity.getComponent<ProjectileComponent>();
                if(SDL_GetTicks() - projectile.startTime > projectile.duration) {
                    entity.kill();
                }
            }
        }
};

#endif
