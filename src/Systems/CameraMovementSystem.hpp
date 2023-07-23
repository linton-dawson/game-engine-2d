#ifndef CAMERAMOVEMENTSYSTEM_HPP
#define CAMERAMOVEMENTSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/CameraFollowComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include <SDL2/SDL.h>

class CameraMovementSystem : public System{
    public:
        CameraMovementSystem() {
            requireComponent<CameraFollowComponent>();
            requireComponent<TransformComponent>();
        };

        void update(SDL_Rect& camera) {
            for(auto entity: getSystemEntities()) {
                auto transform = entity.getComponent<TransformComponent>();
                camera.x = transform.position.x;
                camera.y = transform.position.y;
            }
        }
};

#endif
