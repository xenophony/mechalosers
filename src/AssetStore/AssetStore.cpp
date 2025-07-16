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
    "./src/AssetStore/images/armor.png",
    "./src/AssetStore/images/trooper.png"           // 13
};

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called.");
    textures.resize(textureFileNames.size());
}
AssetStore::~AssetStore() {
    // ClearAssets();
    freeTextures();
    Logger::Log("AssetStore destructor called.");
}

color_t* convertToPixels(SDL_Surface* formattedSurface) {
    if (formattedSurface == nullptr) {
        Logger::Log("Unable to convert surface format for " + std::string(SDL_GetError()));
        return nullptr;
    }

    // Check if we can use direct pixel access (optimization)
    bool canUseDirect = (formattedSurface->format->format == SDL_PIXELFORMAT_RGBA8888);

    color_t* pixelBuffer;
    bool ownPixels = false;

    if (canUseDirect && formattedSurface->format->Rmask == 0xFF000000) {
        // Direct access - format matches our expectation (ARGB)
        Logger::Log("Using direct pixel access for ");
        pixelBuffer = (color_t*)formattedSurface->pixels;
    } else {
        // Manual conversion needed
        Logger::Log("Converting pixel format for ");
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
    return pixelBuffer;
}

void AssetStore::loadTextures() {
    // Initialize SDL_image if not already done
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        Logger::Log("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        return;
    }

    for (size_t i = 0; i < textureFileNames.size(); i++) {
        // Load image as SDL_Surface
        SDL_Surface* loadedSurface = IMG_Load(textureFileNames[i].c_str());
        if (loadedSurface == nullptr) {
            Logger::Log("Unable to load image " + textureFileNames[i] + "! SDL_image Error: " + std::string(IMG_GetError()));
            continue;
        }

        // Convert surface to ARGB8888 format for consistent pixel access
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0);

        std::vector<std::vector<color_t*>> pixelsMatrix(7, std::vector<color_t*>(8));
        if (formattedSurface->w > 500) {
            for (int row = 0; row < pixelsMatrix.size(); row++) {
                for (int col = 0; col < pixelsMatrix[row].size(); col++) {
                    int width = 64;
                    int height = 64;

                    SDL_Rect srcRect;
                    srcRect.x = row * 65;
                    srcRect.y = col * 65;
                    srcRect.w = width;
                    srcRect.h = height;

                    SDL_Surface* cutSurface = SDL_CreateRGBSurfaceWithFormat(
                        0,
                        64,
                        64,
                        formattedSurface->format->BitsPerPixel,
                        formattedSurface->format->format
                    );

                    if (cutSurface == nullptr) {
                        Logger::Log("Failed to create cut surface for frame (" + std::to_string(row) + ", " + std::to_string(col) + ")");
                        continue;
                    }

                    SDL_BlitSurface(formattedSurface, &srcRect, cutSurface, nullptr);
                    pixelsMatrix[row][col] = convertToPixels(cutSurface);
                    SDL_FreeSurface(cutSurface); // Free the cut surface

                }
            }
        }

        SDL_FreeSurface(loadedSurface); // Free the original surface
        // std::vector<std::vector<SDL_Surface*>> surfaceMatrix(7, std::vector<SDL_Surface*>(8));


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
        textureData.pixelsMatrix = pixelsMatrix;
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

int AssetStore::GetTextureWidth(int textureIndex) {
    if (textureIndex >= 0 && textureIndex < NUM_TEXTURES && textureIndex < textures.size()) {
        return textures[textureIndex].width;
    }
    return 0; // Return 0 if invalid index
}

int AssetStore::GetTextureHeight(int textureIndex) {
    if (textureIndex >= 0 && textureIndex < NUM_TEXTURES && textureIndex < textures.size()) {
        return textures[textureIndex].height;
    }
    return 0; // Return 0 if invalid index
}

color_t* AssetStore::GetTexturePixels(int textureIndex) {
    if (textureIndex >= 0 && textureIndex < NUM_TEXTURES && textureIndex < textures.size()) {
        return textures[textureIndex].pixels;
    }
    return nullptr; // Return nullptr if invalid index
}

color_t* AssetStore::GetTexturePixelsAt(int textureIndex, int row, int col) {
    if (textureIndex >= 0 && textureIndex < NUM_TEXTURES && textureIndex < textures.size() &&
        row < textures[textureIndex].pixelsMatrix.size() && col < textures[textureIndex].pixelsMatrix[row].size()) {
        return textures[textureIndex].pixelsMatrix[row][col];
    }
    return nullptr; // Return nullptr if invalid index
}
