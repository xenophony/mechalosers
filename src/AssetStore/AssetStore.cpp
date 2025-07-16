#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>
#include <vector>
#include <string>

std::vector<std::string> textureFileNames = {
    "./src/AssetStore/images/redbrick.png",        // 0
    "./src/AssetStore/images/purplestone.png",     // 1
    "./src/AssetStore/images/mossystone.png",      // 2
    "./src/AssetStore/images/graystone.png",       // 3
    "./src/AssetStore/images/colorstone.png",      // 4
    "./src/AssetStore/images/bluestone.png",       // 5
    "./src/AssetStore/images/wood.png",            // 6
    "./src/AssetStore/images/eagle.png",           // 7
    "./src/AssetStore/images/pikuma.png",          // 8
    "./src/AssetStore/images/barrel.png",          // 9
    "./src/AssetStore/images/light.png",           // 10
    "./src/AssetStore/images/table.png",           // 11
    "./src/AssetStore/images/guard.png",           // 12
    "./src/AssetStore/images/armor.png"            // 13
};

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called.");
    textures.resize(NUM_TEXTURES);
}
AssetStore::~AssetStore() {
    // ClearAssets();
    freeTextures();
    Logger::Log("AssetStore destructor called.");
}

void AssetStore::loadTextures() {
    // Initialize SDL_image if not already done
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        Logger::Log("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        return;
    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        // Load image as SDL_Surface
        SDL_Surface* loadedSurface = IMG_Load(textureFileNames[i].c_str());
        if (loadedSurface == nullptr) {
            Logger::Log("Unable to load image " + textureFileNames[i] + "! SDL_image Error: " + std::string(IMG_GetError()));
            continue;
        }

        // Convert surface to ARGB8888 format for consistent pixel access
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(loadedSurface); // Free the original surface

        if (formattedSurface == nullptr) {
            Logger::Log("Unable to convert surface format for " + textureFileNames[i] + "! SDL Error: " + std::string(SDL_GetError()));
            continue;
        }

        // Check if we can use direct pixel access (optimization)
        bool canUseDirect = (formattedSurface->format->format == SDL_PIXELFORMAT_RGBA8888);

        color_t* pixelBuffer;
        bool ownPixels = false;

        if (canUseDirect && formattedSurface->format->Rmask == 0xFF000000) {
            // Direct access - format matches our expectation (ARGB)
            Logger::Log("Using direct pixel access for " + textureFileNames[i]);
            pixelBuffer = (color_t*)formattedSurface->pixels;
        } else {
            // Manual conversion needed
            Logger::Log("Converting pixel format for " + textureFileNames[i]);
            int pixelCount = formattedSurface->w * formattedSurface->h;
            pixelBuffer = new color_t[pixelCount];
            ownPixels = true;

            SDL_LockSurface(formattedSurface);
            Uint32* sdlPixels = (Uint32*)formattedSurface->pixels;

            for (int p = 0; p < pixelCount; p++) {
                Uint32 pixel = sdlPixels[p];
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixel, formattedSurface->format, &r, &g, &b, &a);

                // Convert to our color format: 0xAARRGGBB - swap R and B to fix blue tint
                pixelBuffer[p] = (a << 24) | (b << 16) | (g << 8) | r;
            }

            SDL_UnlockSurface(formattedSurface);
        }

        // Store texture data
        TextureData textureData;
        textureData.width = formattedSurface->w;
        textureData.height = formattedSurface->h;
        textureData.pixels = pixelBuffer;
        textureData.surface = formattedSurface; // Keep reference for cleanup
        textureData.ownPixels = ownPixels;

        textures[i] = textureData;

        Logger::Log("Loaded texture: " + textureFileNames[i] + " (" +
            std::to_string(textureData.width) + "x" + std::to_string(textureData.height) + ")");
    }
}

void AssetStore::freeTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (textures[i].surface != nullptr) {
            SDL_FreeSurface(textures[i].surface);
            textures[i].surface = nullptr;
        }
        if (textures[i].ownPixels && textures[i].pixels != nullptr) {
            delete[] textures[i].pixels;
            textures[i].pixels = nullptr;
        }
    }
    IMG_Quit(); // Clean up SDL_image
}

// void AssetStore::ClearAssets()
// {
//     for (auto texture : textures) {
//         SDL_DestroyTexture(texture.second);
//     }
//     textures.clear();

//     for (auto font : fonts) {
//         TTF_CloseFont(font.second);
//     }
//     fonts.clear();
// }

// void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
//     SDL_Surface* surface = IMG_Load(filePath.c_str());
//     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//     SDL_FreeSurface(surface);

//     //Add texture to the map
//     textures.emplace(assetId, texture);

//     Logger::Log("New asset added to the asset store with assetID = (" + assetId + ")");
// }

// void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
//     fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
// }





// TTF_Font* AssetStore::GetFont(const std::string& assetId) {
//     return fonts[assetId];
// }






// SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {

//     return textures[assetId];

// }
