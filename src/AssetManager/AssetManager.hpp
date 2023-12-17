#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AssetManager {
    private:
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
    public:
        AssetManager();
        ~AssetManager();

        void clearAssets();
        void addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        SDL_Texture* getTexture(const std::string& assetId) const;
        
        //TODO: Use std::filesystem for filepath
        void addFont(const std::string& assetId, const std::string& filePath, int fontsize);
        TTF_Font* getFont(const std::string& assetId);

};

#endif
