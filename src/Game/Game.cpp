#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.hpp"

Game::Game() { Logger::Log("constructed game object");}
Game::~Game() { Logger::Log("game object destroyed");}
void Game::Init() {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    window = SDL_CreateWindow("jabardast window", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
    if(!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) { std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}
void Game::Run() {
    Setup();
    while(isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}
void Game::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;
        }
    }
}
glm::vec2 playerPos;
glm::vec2 playerVelocity;
void Game::Setup() {
    playerPos = glm::vec2(10,20);
    playerVelocity = glm::vec2(100, 0);
}
void Game::Update() {

    //capping game loop to 60fps by holding till end of frame
    //and then releasing thread
    signed int delayDuration = SDL_GetTicks() - previousTick + ms_per_frame;
    if(delayDuration <= ms_per_frame){
        SDL_Delay(delayDuration);
    }
    
    // Duration since last frame in seconds
    double deltaTime = static_cast<double>(SDL_GetTicks() - previousTick)/1000;

    previousTick = SDL_GetTicks();
    playerPos.x += playerVelocity.x * deltaTime;
    playerPos.y += playerVelocity.y * deltaTime;
}
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    //draw PNG texture
    auto surface = IMG_Load("./assets/walter_white.png");
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dstRect = {static_cast<int>(playerPos.x), static_cast<int>(playerPos.y), 32, 32};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}
void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
