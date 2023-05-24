#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include <SDL2/SDL.h>
#include "../ECS/ECS.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/AnimationComponent.hpp"

// TODO: Implement animation grouping by name

class AnimationSystem : public System {
    public:
        AnimationSystem() {
            requireComponent<SpriteComponent>();
            requireComponent<AnimationComponent>();
        }

        void update() {
            for(auto entity : getSystemEntities()) {
                auto& animation = entity.getComponent<AnimationComponent>();
                auto& sprite = entity.getComponent<SpriteComponent>();

                animation.currFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate/1000) % animation.numFrames;
                sprite.srcRect.x = animation.currFrame * sprite.w;

            }
        }
};

#endif
