#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetManager {
    private:
        std::map<std::string, SDL_Texture*> textures;
    public:
        AssetManager();
        ~AssetManager();

        void clearAssets();
        void addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        SDL_Texture* getTexture(const std::string& assetId) const;

};

#endif
