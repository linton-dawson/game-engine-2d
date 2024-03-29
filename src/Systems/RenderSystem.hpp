#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP 

#include <SDL2/SDL.h>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Logger/Logger.hpp"
#include "../ECS/ECS.hpp"

class RenderSystem: public System {
    public:
        RenderSystem() {
            requireComponent<TransformComponent>();
            requireComponent<SpriteComponent>();
        }
        //TODO Sort entities by their SpriteComponent's z-index
        void update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
            for (auto entity : getSystemEntities()) {
                const auto transform = entity.getComponent<TransformComponent>();
                const auto sprite = entity.getComponent<SpriteComponent>();

                //set source rect for render copy
                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect destRect = {static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)), static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)), static_cast<int>(sprite.w * transform.scale.x), static_cast<int>(sprite.h * transform.scale.y)};
                SDL_RenderCopyEx(renderer, assetManager->getTexture(sprite.assetId), &srcRect, &destRect, transform.rotation, nullptr, SDL_FLIP_NONE);
                //TODO: Move this part to a separate system to render hitboxes
//                const SDL_Rect tmp = SDL_Rect(static_cast<int>(transform.position.x - camera.x), static_cast<int>(transform.position.y - camera.y), static_cast<int>(sprite.w), static_cast<int>(sprite.h));
//                if(entity.hasComponent<BoxColliderComponent>()) {
//                    SDL_SetRenderDrawColor(renderer,255,255,0,0);
//                    SDL_RenderDrawRect(renderer, &tmp);
//                }
            }
        }

};
#endif
