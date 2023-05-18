#include "Game.hpp"
#include <exception>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <optional>
#include <string>
#include <utility>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../ECS/ECS.hpp"

Game::Game() { 
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetManager = std::make_unique<AssetManager>();
    Logger::Log("constructed game object");
}
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

std::optional<std::pair<std::vector<int>, uint8_t>>_readTileMap(std::filesystem::path mapPath) {
    try {
        std::ifstream readMap(mapPath, std::ios::in);
        std::ostringstream s;
        s << readMap.rdbuf();
        Logger::Log(s.str());
        std::vector<int> tileMap;
        bool rowsCalculated = false;
        uint8_t colElementNum = 0;
        auto setTotalCols = [&colElementNum, &rowsCalculated](int sz){colElementNum = sz;
            rowsCalculated = true;
        };
        std::string num{};
        for(auto c : s.str()) {
            if(std::isdigit(c)) {
                num += c;
            }
            else if(std::ispunct(c) || c == '\n') {
                tileMap.emplace_back(std::stoi(num));
                std::cout<<num<<",";
                if(!rowsCalculated && c == '\n') {
                    setTotalCols(tileMap.size());
                }
                num.clear();
            }
        }
        Logger::Log("TOTAL NUMBER OF ROWS: "+std::to_string(colElementNum));
        return std::optional<std::pair<std::vector<int>, uint8_t>>{std::make_pair(tileMap, colElementNum)};
    }
    catch(std::exception& e) {
        Logger::Err(e.what());
        return std::nullopt;
    }

}

// loader function for render
void Game::LoadLevel(int level) {
    //creating systems
    registry->addSystem<RenderSystem>();
    
    //adding assets to the asset manager
    assetManager->addTexture(renderer, "jungle", "./assets/tilemaps/jungle.png");

    auto tileObj = _readTileMap(std::filesystem::path("./assets/tilemaps/jungle.map"));
    std::vector<Entity> tilemapEntities;
    tilemapEntities.reserve(30);
    int y = 0;
    for(int tileIndex = 0; tileIndex < static_cast<int>(tileObj.value().first.size()); ++tileIndex) {
        if(tileIndex && tileIndex% tileObj.value().second == 0) {
            Logger::Log("ADDING NEW ROW");
            y++;
        }
        int x = tileIndex % tileObj.value().second;
        auto tileNumber = tileObj.value().first.at(tileIndex);
        Entity tile = registry->createEntity();
        tile.addComponent<TransformComponent>(glm::vec2(32*x, 32*y), glm::vec2(1.0,1.0),0.0);
        tile.addComponent<SpriteComponent>("jungle",32, 32, 32*(tileNumber%10), 32*(tileNumber/10));
        Logger::Log("ROW NUMBER: " + std::to_string(y));
        Logger::Log("TILE NUMBER: " + std::to_string(tileObj.value().first.at(tileIndex)));
        Logger::Log("TILE POS: " + std::to_string(32*(tileNumber%10)) + "," + std::to_string(32*(tileNumber/10)));
        Logger::Log("TILEMAP index: " + std::to_string(x) + "," + std::to_string(y));
        
    }

}
void Game::Setup() {
    LoadLevel(1);
}
void Game::Update() {

    //capping game loop to 60fps by holding till end of frame
    //and then releasing thread
    signed int delayDuration = SDL_GetTicks() - previousTick + ms_per_frame;
    if(delayDuration <= ms_per_frame){
        SDL_Delay(delayDuration);
    }
    
    // Duration since last frame in seconds
//    double deltaTime = static_cast<double>(SDL_GetTicks() - previousTick)/1000;

    previousTick = SDL_GetTicks();

    // update registry queued actions
    registry->update();

    //registry->getSystem<MovementSystem>().update(deltaTime);
}
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);
    
    registry->getSystem<RenderSystem>().update(renderer, assetManager);
    SDL_RenderPresent(renderer);
}
void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
