
#ifndef RAYSRENDERSYSTEM_H
#define RAYSRENDERSYSTEM_H

#include <entt/entt.hpp>
// #include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include "../Components/GridComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RaysComponent.h"
#include <SDL2/SDL.h>
#include "../defs.h"
#include "../Utils/Utils.h"
#include "../../libs/upng/upng.h"
#include "../AssetStore/AssetStore.h"

class RaysRenderSystem {
public:

    void changeColorIntensity(color_t* color, float factor) {
        color_t a = (*color & 0xFF000000);
        color_t r = (*color & 0x00FF0000) * factor;
        color_t g = (*color & 0x0000FF00) * factor;
        color_t b = (*color & 0x000000FF) * factor;

        *color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    }

    void Render(entt::registry& registry, SDL_Renderer* renderer, color_t* colorBuffer, std::unique_ptr <AssetStore>& assetStore) {
        std::vector<upng_t*> textures = assetStore->textures;

        auto view = registry.view<RaysComponent, TransformComponent>();

        for (auto entity : view) {
            const auto& transform = view.get<TransformComponent>(entity);
            const auto& rays = view.get<RaysComponent>(entity);
            for (int rayIndex = 0; rayIndex < rays.rays.size(); rayIndex++) {
                float perpDistance = rays.rays[rayIndex].distance * cos(rays.rays[rayIndex].rayAngle - transform.rotation);
                float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

                int wallHeight = (int)projectedWallHeight;
                int wallTopY = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
                wallTopY = wallTopY < 0 ? 0 : wallTopY;

                int wallBottomY = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
                wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

                int texNum = rays.rays[rayIndex].texture - 1;
                int texHeight = upng_get_height(textures[texNum]);
                int texWidth = upng_get_width(textures[texNum]);
                color_t* wallTextureBuffer = (color_t*)upng_get_buffer(textures[texNum]);

                // Calculate texture X coordinate for this ray
                // Use consistent texture coordinate across the entire ray strip
                int textureOffsetX;
                if (rays.rays[rayIndex].wasHitVertical) {
                    textureOffsetX = (int)rays.rays[rayIndex].wallHitY % TILE_SIZE;
                } else {
                    textureOffsetX = (int)rays.rays[rayIndex].wallHitX % TILE_SIZE;
                }

                // Scale texture coordinate to actual texture dimensions
                textureOffsetX = (textureOffsetX * texWidth) / TILE_SIZE;

                // Clamp to prevent array bounds issues
                if (textureOffsetX < 0) textureOffsetX = 0;
                if (textureOffsetX >= texWidth) textureOffsetX = texWidth - 1;

                // Draw this ray across RAY_WIDTH screen columns
                for (int stripCol = 0; stripCol < RAY_WIDTH; stripCol++) {
                    int x = rayIndex * RAY_WIDTH + stripCol;
                    if (x >= WINDOW_WIDTH) break; // Prevent drawing outside screen bounds

                    for (int y = 0; y < wallTopY; y++) {
                        drawPixel(colorBuffer, x, y, 0xFF444444);
                    }

                    for (int y = wallTopY; y < wallBottomY; y++) {
                        int distanceFromTop = y + (wallHeight / 2) - (WINDOW_HEIGHT / 2);
                        int textureOffsetY = distanceFromTop * ((float)texHeight / wallHeight);

                        // Clamp texture coordinates to prevent segfaults
                        if (textureOffsetY < 0) textureOffsetY = 0;
                        if (textureOffsetY >= texHeight) textureOffsetY = texHeight - 1;

                        color_t texelColor = wallTextureBuffer[(texWidth * textureOffsetY) + textureOffsetX];

                        if (rays.rays[rayIndex].wasHitVertical) {
                            changeColorIntensity(&texelColor, 0.7);
                        }

                        drawPixel(colorBuffer, x, y, texelColor);
                    }

                    for (int y = wallBottomY + 1; y < WINDOW_HEIGHT; y++) {
                        drawPixel(colorBuffer, x, y, 0xFF888888);
                    }
                } // End of stripCol loop
            } // End of rayIndex loop
        }
    }
};

#endif





