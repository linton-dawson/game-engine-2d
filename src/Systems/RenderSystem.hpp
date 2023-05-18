#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP 

#include <SDL2/SDL.h>
#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Logger/Logger.hpp"

class RenderSystem: public System {
    public:
        RenderSystem() {
            requireComponent<TransformComponent>();
            requireComponent<SpriteComponent>();
        }

        void update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager) {
            for (auto entity : getSystemEntities()) {
                const auto transform = entity.getComponent<TransformComponent>();
                const auto sprite = entity.getComponent<SpriteComponent>();

                //set source rect for render copy
                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect destRect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), static_cast<int>(sprite.w * transform.scale.x), static_cast<int>(sprite.h * transform.scale.y)};
                SDL_RenderCopyEx(renderer, assetManager->getTexture(sprite.assetId), &srcRect, &destRect, transform.rotation, nullptr, SDL_FLIP_NONE);
            }
        }

};
#endif
