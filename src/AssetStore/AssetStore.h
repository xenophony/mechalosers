#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../defs.h"
#include <stdint.h>
#include <vector>

struct TextureData {
    int width;
    int height;
    color_t* pixels;
    std::vector<std::vector<color_t*>> pixelsMatrix;
    SDL_Surface* surface; // Keep reference for cleanup
    bool ownPixels; // Flag to track if we allocated pixels ourselves
};

class AssetStore {
private:

    // std::map<std::string, SDL_Texture*> textures;
    // std::map<std::string, TTF_Font*> fonts;
    // TODO: map for fonts and audio
public:
    std::vector<TextureData> textures;
    AssetStore();
    ~AssetStore();

    // void ClearAssets();

    void loadTextures();
    void freeTextures();

    // Getter methods for texture dimensions
    int GetTextureWidth(int textureIndex);
    int GetTextureHeight(int textureIndex);

    // Getter method for texture pixel buffer
    color_t* GetTexturePixels(int textureIndex);
    color_t* GetTexturePixelsAt(int textureIndex, int row, int col);
    // void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    // SDL_Texture* GetTexture(const std::string& assetId);

    // void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);

    // TTF_Font* GetFont(const std::string& assetId);
};

#endif