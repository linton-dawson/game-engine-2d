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
#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/KeyboardControlledComponent.hpp"
#include "../Components/CameraFollowComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/HealthComponent.hpp"
#include "../Components/TextLabelComponent.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/CollisionSystem.hpp"
#include "../Systems/AnimationSystem.hpp"
#include "../Systems/DamageSystem.hpp"
#include "../Systems/KeyboardMovementSystem.hpp"
#include "../Systems/CameraMovementSystem.hpp"
#include "../Systems/ProjectileEmitSystem.hpp"
#include "../Systems/ProjectileLifecycleSystem.hpp"
#include "../Systems/RenderTextSystem.hpp"
#include "../Systems/RenderHealthBarSystem.hpp"
#include "../ECS/ECS.hpp"

int Game::windowWidth, Game::windowHeight, Game::mapWidth, Game::mapHeight;

Game::Game() { 
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetManager = std::make_unique<AssetManager>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("constructed game object");
}
Game::~Game() { Logger::Log("game object destroyed");}
void Game::Init() {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }
    if(TTF_Init() != 0) {
        std::cerr << "Error initializing TTF" << std::endl;
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow("jabardast window", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    if(!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) { std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

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
                eventBus->emitEvent<KeyPressedEvent>(event.key.keysym.sym);
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
        readMap.close();
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
    registry->addSystem<MovementSystem>();
    registry->addSystem<AnimationSystem>();
    registry->addSystem<CollisionSystem>();
    registry->addSystem<DamageSystem>();
    registry->addSystem<KeyboardMovementSystem>();
    registry->addSystem<CameraMovementSystem>();
    registry->addSystem<ProjectileEmitSystem>();
    registry->addSystem<ProjectileLifecycleSystem>();
    registry->addSystem<RenderTextSystem>();
    registry->addSystem<RenderHealthBarSystem>();

    //adding assets to the asset manager
    assetManager->addTexture(renderer, "tank", "./assets/images/tank-panther-right.png");
    assetManager->addTexture(renderer, "chopper", "./assets/images/chopper-spritesheet.png");
    assetManager->addTexture(renderer, "radar", "./assets/images/radar.png");
    assetManager->addTexture(renderer, "jungle", "./assets/tilemaps/jungle.png");
    assetManager->addTexture(renderer, "bullet", "./assets/images/bullet.png");
    assetManager->addFont("charriot","./assets/fonts/charriot.ttf",5);
    assetManager->addFont("arial","./assets/fonts/arial.ttf",10);

    auto tileObj = _readTileMap(std::filesystem::path("./assets/tilemaps/jungle.map"));
    int y = 0;
    for(int tileIndex = 0; tileIndex < static_cast<int>(tileObj.value().first.size()); ++tileIndex) {
        if(tileIndex && tileIndex% tileObj.value().second == 0) {
            y++;
        }
        int x = tileIndex % tileObj.value().second;
        auto tileNumber = tileObj.value().first.at(tileIndex);
        Entity tile = registry->createEntity();
        tile.group("tiles");
        tile.addComponent<TransformComponent>(glm::vec2(32*2*x, 32*2*y), glm::vec2(2.0,2.0),0.0);
        tile.addComponent<SpriteComponent>("jungle",32, 32, 32*(tileNumber%10), 32*(tileNumber/10));
    }
    //TODO: Add tilemap scale. currently it's 2.
    //TODO: add 32 as tileSize variable instead of raw value
    mapWidth = tileObj.value().second * 32 * 2;
//    mapHeight = tileObj.value().first.size();
    mapHeight = 1280;
    Logger::Log("mapWidth = " + std::to_string(mapWidth) + " mapHeight = " + std::to_string(mapHeight));
    Logger::Log("windowWidth = " + std::to_string(windowWidth) + " windowHeight = " + std::to_string(windowHeight));

    // chopper
    Entity chopper = registry->createEntity();
    chopper.tag("player");
    chopper.addComponent<TransformComponent>(glm::vec2(10.0,100.0), glm::vec2(1,1), 0.0);
    chopper.addComponent<SpriteComponent>("chopper",32,32);
    chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.addComponent<AnimationComponent>(2, 15, true);    
    chopper.addComponent<BoxColliderComponent>(32,32); 
    chopper.addComponent<HealthComponent>(100);
    chopper.addComponent<KeyboardControlledComponent>(glm::vec2(0,-70), glm::vec2(70,0), glm::vec2(0,70), glm::vec2(-70,0));
    chopper.addComponent<CameraFollowComponent>();
    chopper.addComponent<ProjectileEmitterComponent>(glm::vec2(150.0,150.0),0,10000,10,true);

    // radar&
    Entity radar = registry->createEntity();
    radar.addComponent<TransformComponent>(glm::vec2(windowWidth - 84,10.0), glm::vec2(1,1), 0.0);
    radar.addComponent<SpriteComponent>("radar",64,64,2,0,0,true);
    radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.addComponent<AnimationComponent>(8, 7, true);
    
    //tank
    Entity tank = registry->createEntity();
    tank.group("enemies");
    tank.addComponent<TransformComponent>(glm::vec2(500.0,10.0), glm::vec2(1,1), 0.0);
    tank.addComponent<SpriteComponent>("tank",32,32);
    tank.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.addComponent<BoxColliderComponent>(32,32);
    tank.addComponent<HealthComponent>(100);
    tank.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0), 5000, 3000, 50, false);

    Entity label = registry->createEntity();
    SDL_Color color = {255,255,255};
    label.addComponent<TextLabelComponent>(glm::vec2(windowWidth/2 - 40,10), "TEST GAME v0", "charriot", color, true);
    
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
    double deltaTime = static_cast<double>(SDL_GetTicks() - previousTick)/1000;

    previousTick = SDL_GetTicks();

    eventBus->reset();

    // perform subscription of events for all systems
    registry->getSystem<DamageSystem>().subscribeToEvents(eventBus);
    registry->getSystem<KeyboardMovementSystem>().subscribeToEvents(eventBus);
    registry->getSystem<ProjectileEmitSystem>().subscribeToEvents(eventBus);
    
    // update registry queued actions
    registry->update();

    registry->getSystem<MovementSystem>().update(deltaTime);
    registry->getSystem<AnimationSystem>().update();
    registry->getSystem<CollisionSystem>().update(eventBus);
    registry->getSystem<CameraMovementSystem>().update(camera);
    registry->getSystem<ProjectileEmitSystem>().update(registry);
    registry->getSystem<ProjectileLifecycleSystem>().update();
}
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);
    
    registry->getSystem<RenderSystem>().update(renderer, assetManager, camera);
    registry->getSystem<RenderTextSystem>().update(renderer, assetManager, camera);
    registry->getSystem<RenderHealthBarSystem>().update(renderer, assetManager, camera);
    SDL_RenderPresent(renderer);
}
void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
