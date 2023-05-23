#ifndef BOXCOLLIDERCOMPONENT_HPP
#define BOXCOLLIDERCOMPONENT_HPP

#include <glm/glm.hpp>

struct BoxColliderComponent {
    int w, h;
    glm::vec2 offset;
    BoxColliderComponent(int w = 0, int h = 0, glm::vec2 offset = glm::vec2(0)) {
        this->w = w;
        this->h = h;
        this->offset = offset;
    }
};
#endif
