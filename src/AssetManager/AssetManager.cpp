#include "./AssetManager.hpp"
#include "../Logger/Logger.hpp"
#include <SDL2/SDL_image.h>

AssetManager::AssetManager() {
    Logger::Log("Creating asset manager");
};

AssetManager::~AssetManager() {
    clearAssets();
    Logger::Log("Destroying asset manager");
}

void AssetManager::clearAssets() {
    for(auto t : textures) {
        SDL_DestroyTexture(t.second);
    }
    textures.clear();
}

void AssetManager::addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
}

SDL_Texture* AssetManager::getTexture(const std::string& assetId) const {
    return textures.at(assetId);
}
