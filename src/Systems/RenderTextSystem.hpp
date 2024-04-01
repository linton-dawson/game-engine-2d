#ifndef RENDERTEXTSYSTEM_HPP
#define RENDERTEXTSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include <SDL2/SDL.h>
#include "../Components/TextLabelComponent.hpp"
#include "../AssetManager/AssetManager.hpp"

class RenderTextSystem : public System {
    public:
        RenderTextSystem() {
            requireComponent<TextLabelComponent>();
        }

        void update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera) {
            for(auto entity : getSystemEntities()) {
                const auto textLabel = entity.getComponent<TextLabelComponent>();
                SDL_Surface* surface = TTF_RenderText_Blended(assetManager->getFont(textLabel.assetId), textLabel.text.c_str(), textLabel.color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                int labelWidth = 0, labelHeight = 0;
                SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);
                SDL_Rect destRect = {static_cast<int>(textLabel.pos.x - (textLabel.isFixed ? 0 : camera.x)), static_cast<int>(textLabel.pos.y - (textLabel.isFixed ? 0 : camera.y)), labelWidth, labelHeight};
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
                SDL_DestroyTexture(texture);
            }
        }
};

#endif
