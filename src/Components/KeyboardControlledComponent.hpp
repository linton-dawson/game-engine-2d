#ifndef KEYBOARDCONTROLLEDCOMPONENT_HPP
#define KEYBOARDCONTROLLEDCOMPONENT_HPP

#include <glm/glm.hpp>

struct KeyboardControlledComponent {
    glm::vec2 up, right, down, left;
    KeyboardControlledComponent(glm::vec2 up = glm::vec2(0), glm::vec2 right = glm::vec2(0), glm::vec2 down = glm::vec2(0), glm::vec2 left = glm::vec2(0)){
            this->up = up;
            this->right = right;
            this->down = down;
            this->left = left;
    }
};

#endif
