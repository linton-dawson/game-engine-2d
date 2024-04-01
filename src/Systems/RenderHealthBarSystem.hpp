#ifndef RENDERHEALTHBARSYSTEM_HPP
#define RENDERHEALTHBARSYSTEM_HPP


#include "../ECS/ECS.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include <SDL2/SDL.h>

class RenderHealthBarSystem: public System {
    public:
        RenderHealthBarSystem() {
            requireComponent<TransformComponent>();
            requireComponent<SpriteComponent>();
            requireComponent<HealthComponent>();
        } 

        void update(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera) {

            //TODO: Fix this. Health text currently detached from the player sprite
            for(auto entity : getSystemEntities()) {
                const auto transform = entity.getComponent<TransformComponent>();
                const auto sprite = entity.getComponent<SpriteComponent>();
                const auto health = entity.getComponent<HealthComponent>();

                SDL_Color barColor = {255,255,255};
                if(health.health >= 0 && health.health < 40) {
                    barColor = {255,0,0};
                }
                else if(health.health >= 40 && health.health < 80) {
                    barColor = {255 ,255, 0};
                }
                else if(health.health >= 80) {
                    barColor = {0, 255, 0};
                }

                int barWidth = 15;
                int barHeight = 3;
                double barX = (transform.position.x + (sprite.w * transform.scale.x)) - camera.x;
                double barY = (transform.position.y) - camera.y;
                SDL_Rect healthBarRect = {
                    static_cast<int>(barX),
                    static_cast<int>(barY),
                    static_cast<int>(barWidth * (static_cast<double>(health.health)/100)),
                    static_cast<int>(barHeight)
                };
                SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, 255);
                SDL_RenderFillRect(renderer, &healthBarRect);

                // text percent label
                std::string healthText = std::to_string(health.health);
                SDL_Surface* surface = TTF_RenderText_Blended(assetManager->getFont("arial"), healthText.c_str(), barColor);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                int labelWidth = 0;
                int labelHeight = 0;
                SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);
                SDL_Rect healthBarTextRect = {
                    static_cast<int>(barX),
                    static_cast<int>(barY)* 5,
                    labelWidth,
                    labelHeight
                };
                SDL_RenderCopy(renderer, texture, nullptr, &healthBarTextRect);
                SDL_DestroyTexture(texture);
            }
        } 
};
#endif
