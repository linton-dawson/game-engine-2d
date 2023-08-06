#ifndef SPRITECOMPONENT_HPP 
#define SPRITECOMPONENT_HPP 

#include <SDL2/SDL.h>
#include<string>
struct SpriteComponent {
    std::string assetId;
    int w,h, zIndex;
    SDL_Rect srcRect;
    bool isFixed;
    
    SpriteComponent(std::string assetId = "", int w = 0, int h = 0, int srcRectX = 0, int srcRectY = 0, int zIndex=0, bool isFixed = false) {
        this->assetId = assetId;
        this->w = w;
        this->h = h;
        this->srcRect = {srcRectX, srcRectY, w, h};
        this->zIndex = zIndex;
        this->isFixed = isFixed;
    }
};

#endif
