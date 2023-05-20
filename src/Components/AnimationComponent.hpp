#ifndef CHOPPERCOMPONENT_HPP
#define CHOPPERCOMPONENT_HPP

#include <SDL2/SDL.h>

struct AnimationComponent {
    int numFrames;
    int currFrame;
    int frameSpeedRate;
    bool isLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true) {
        this->numFrames = numFrames;
        this->currFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
    }
};

#endif
