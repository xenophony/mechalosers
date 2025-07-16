#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../defs.h"
#include <stdint.h>
#include "../../libs/upng/upng.h"
#include <vector>

class AssetStore {
private:

    // std::map<std::string, SDL_Texture*> textures;
    // std::map<std::string, TTF_Font*> fonts;
    // TODO: map for fonts and audio
public:
    std::vector<upng_t*> textures;
    AssetStore();
    ~AssetStore();

    // void ClearAssets();



    void loadTextures();
    void freeTextures();
    // void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    // SDL_Texture* GetTexture(const std::string& assetId);

    // void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);

    // TTF_Font* GetFont(const std::string& assetId);
};

#endif