#ifndef KEYPRESSEDEVENT_HPP
#define KEYPRESSEDEVENT_HPP

#include "../ECS/ECS.hpp"
#include "../EventBus/Event.hpp"
#include <SDL2/SDL.h>

class KeyPressedEvent: public Event{
    public:
        SDL_Keycode symbol;
        KeyPressedEvent(SDL_Keycode symbol): symbol(symbol) {}
};

#endif
