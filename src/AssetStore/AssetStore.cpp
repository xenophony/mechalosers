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
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng;

        upng = upng_new_from_file(textureFileNames[i].c_str());
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                textures[i] = upng;
                // textures[i].width = upng_get_width(upng);
                // textures[i].height= upng_get_width(upng);
                // textures[i].texture_buffer= (color_t*) upng_get_buffer(upng);
            } else if (upng_get_error(upng) == UPNG_ENOTFOUND)
            {
                fprintf(stderr, "cannot find file name: %s\n", textureFileNames[i].c_str());
            }
        }
    }
}

void AssetStore::freeTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_free(textures[i]);
        // wallTextures[i].texture_buffer;
    }
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
