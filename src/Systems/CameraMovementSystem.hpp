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
                
                // TODO: Fix tilemap
                if(transform.position.x + (camera.w/4) < Game::mapWidth) {
                camera.x = transform.position.x - (Game::windowWidth / 4);
                }
                if(transform.position.y + (camera.h/2) < Game::mapHeight) {
                camera.y = transform.position.y - (Game::windowHeight / 2);
                }
                camera.x = std::clamp(camera.x, 0, camera.w);
                camera.y = std::clamp(camera.y, 0, camera.h);
            }
        }
};

#endif
