#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../ECS/ECS.hpp"

const int fps = 60;
const int ms_per_frame = 1000/fps;
class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int previousTick = 0;
        bool isRunning;
        Registry* registry;
    public:
        Game();
        ~Game();
        void Init();
        void Run();
        void ProcessInput();
        void Setup();
        void Update();
        void Render();
        void Destroy();
};
#endif
