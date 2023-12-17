#ifndef TEXTLABELCOMPONENT_HPP
#define TEXTLABELCOMPONENT_HPP

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>

struct TextLabelComponent {
    glm::vec2 pos;
    std::string text;
    std::string assetId;
    SDL_Color color;
    bool isFixed;

    TextLabelComponent(glm::vec2 position = glm::vec2(0), std::string text = "", std::string assetId = "", SDL_Color = {0,0,0}, bool isFixed = false) {
        this->pos = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
    }
};

#endif
