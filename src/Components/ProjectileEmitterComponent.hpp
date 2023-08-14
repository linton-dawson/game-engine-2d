#ifndef PROJECTILEEMITTERCOMPONENT_HPP
#define PROJECTILEEMITTERCOMPONENT_HPP

#include <SDL2/SDL.h>

struct ProjectileEmitterComponent {
    glm::vec2 velocity;
    int frequency;
    int duration;
    int hitPercentDamage;
    bool isFriendly;
    int lastEmitTime;

    ProjectileEmitterComponent(glm::vec2 velocity = glm::vec2(0), int frequency = 1000, int duration = 10000, int hitPercentDamage = 10, bool isFriendly = true) {
        this->velocity = velocity;
        this->frequency = frequency;
        this->duration = duration;
        this->hitPercentDamage = hitPercentDamage;
        this->isFriendly = isFriendly;
        this->lastEmitTime = SDL_GetTicks();
    }
};

#endif
