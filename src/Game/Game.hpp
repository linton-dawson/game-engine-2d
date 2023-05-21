#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <filesystem>
#include "../AssetManager/AssetManager.hpp"
#include "../ECS/ECS.hpp"

const int fps = 60;
const int ms_per_frame = 1000/fps;
class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int previousTick = 0;
        bool isRunning;
        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetManager> assetManager;
        int windowWidth = 800, windowHeight = 600;
    public:
        Game();
        ~Game();
        void Init();
        void Run();
        void ProcessInput();
        void Setup();
        void LoadLevel(int level);
        void Update();
        void Render();
        void Destroy();
};
#endif
