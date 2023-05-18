#ifndef SPRITECOMPONENT_HPP 
#define SPRITECOMPONENT_HPP 

#include <SDL2/SDL.h>
#include<string>
struct SpriteComponent {
    std::string assetId;
    int w,h;
    SDL_Rect srcRect;
    
    SpriteComponent(std::string assetId = "", int w = 0, int h = 0, int srcRectX = 0, int srcRectY = 0) {
        this->assetId = assetId;
        this->w = w;
        this->h = h;
        this->srcRect = {srcRectX, srcRectY, w, h};
    }
};

#endif
