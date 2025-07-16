#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include "../defs.h"
#include "../Components/RaysComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/GridComponent.h"
#include "../AssetStore/AssetStore.h"
#include "../Utils/Utils.h"
#include "../Logger/Logger.h"
#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

class SpriteRenderSystem {
public:
    void Render(entt::registry& registry, SDL_Renderer* renderer, color_t* colorBuffer, std::unique_ptr<AssetStore>& assetStore, entt::entity player) {

        std::vector<entt::entity> visibleSprites;
        visibleSprites.reserve(50);

        const auto& transform = registry.get<TransformComponent>(player);
        const auto& rays = registry.get<RaysComponent>(player);

        auto sprites = registry.view<TransformComponent, SpriteComponent>();

        Logger::Log("SpriteRenderSystem checking sprites");

        for (auto entity : sprites) {
            auto& sprite = sprites.get<SpriteComponent>(entity);
            auto& spriteTransform = sprites.get<TransformComponent>(entity);

            Logger::Log("Found sprite at position (" + std::to_string(spriteTransform.position.x) + ", " + std::to_string(spriteTransform.position.y) + ")");
            Logger::Log("Player at position (" + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y) + ") rotation=" + std::to_string(transform.rotation));

            float angleSpritePlayer = transform.rotation - atan2(spriteTransform.position.y - transform.position.y, spriteTransform.position.x - transform.position.x);

            if (angleSpritePlayer > PI) {
                angleSpritePlayer -= TWO_PI;
            }
            if (angleSpritePlayer < -PI) {
                angleSpritePlayer += TWO_PI;
            }
            angleSpritePlayer = fabs(angleSpritePlayer);

            Logger::Log("Sprite angle relative to player: " + std::to_string(angleSpritePlayer) + " (FOV half-angle: " + std::to_string(FOV_ANGLE / 2) + ")");

            const float EPSILON = 0.1;
            if (angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON) {
                sprite.isVisible = true;
                sprite.angle = angleSpritePlayer;
                spriteTransform.distance = distanceBetweenPoints(
                    spriteTransform.position.x,
                    spriteTransform.position.y,
                    transform.position.x,
                    transform.position.y
                );
                visibleSprites.emplace_back(entity);
                Logger::Log("Sprite is VISIBLE - added to render list");
            } else {
                sprite.isVisible = false;
                Logger::Log("Sprite is NOT visible - outside FOV");
            }
        }

        // Sort visible sprites by distance (farthest to closest for proper depth ordering)
        std::sort(visibleSprites.begin(), visibleSprites.end(), [&registry](entt::entity a, entt::entity b) {
            const auto& transformA = registry.get<TransformComponent>(a);
            const auto& transformB = registry.get<TransformComponent>(b);
            return transformA.distance > transformB.distance; // Render farthest first, closest last
            });

        for (auto entity : visibleSprites) {
            auto& sprite = sprites.get<SpriteComponent>(entity);
            auto& spriteTransform = sprites.get<TransformComponent>(entity);

            Logger::Log("Rendering sprite at distance: " + std::to_string(spriteTransform.distance));

            // Skip rendering if sprite is too close (causes numerical issues)
            if (spriteTransform.distance < 1.0f) {
                Logger::Log("Sprite too close, skipping");
                continue;
            }

            float perpDistance = spriteTransform.distance * cos(sprite.angle);
            float spriteHeight = (sprite.height / perpDistance) * DIST_PROJ_PLANE;
            float spriteWidth = spriteHeight;
            float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
            spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

            float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
            spriteBottomY = (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;


            float spriteAngle = atan2(spriteTransform.position.y - transform.position.y, spriteTransform.position.x - transform.position.x) - transform.rotation;
            float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

            float spriteLeftX = (WINDOW_WIDTH / 2) + spriteScreenPosX - (spriteWidth / 2);
            float spriteRightX = spriteLeftX + spriteWidth;

            Logger::Log("Sprite bounds: left=" + std::to_string(spriteLeftX) + " right=" + std::to_string(spriteRightX) + " top=" + std::to_string(spriteTopY) + " bottom=" + std::to_string(spriteBottomY));
            int textureWidth = sprite.width; //assetStore->GetTextureWidth(sprite.texture);
            int textureHeight = sprite.height; //assetStore->GetTextureHeight(sprite.texture);

            Logger::Log("Texture dimensions for texture " + std::to_string(sprite.texture) + ": " + std::to_string(textureWidth) + "x" + std::to_string(textureHeight));

            for (int x = spriteLeftX; x < spriteRightX; x++) {
                // Skip if x coordinate is outside screen bounds
                if (x < 0 || x >= WINDOW_WIDTH) continue;

                float texelWidth = (textureWidth / spriteWidth);
                int textureOffsetX = (x - spriteLeftX) * texelWidth;

                for (int y = spriteTopY; y < spriteBottomY; y++) {
                    // Skip if y coordinate is outside screen bounds
                    if (y < 0 || y >= WINDOW_HEIGHT) continue;

                    int distanceFromTop = y + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
                    float texelHeight = (textureHeight / spriteHeight);
                    int textureOffsetY = distanceFromTop * texelHeight;

                    // Add bounds checking for texture coordinates
                    if (textureOffsetX < 0 || textureOffsetX >= textureWidth || textureOffsetY < 0 || textureOffsetY >= textureHeight) {
                        continue; // Skip invalid texture coordinates
                    }

                    // Depth test: check if sprite is behind a wall
                    // Convert screen x to ray index for depth comparison
                    int rayIndex = x / RAY_WIDTH;
                    if (rayIndex >= 0 && rayIndex < NUM_RAYS) {
                        float wallDistance = rays.rays[rayIndex].distance;

                        // If sprite is farther than the wall at this ray, it's hidden
                        if (spriteTransform.distance >= wallDistance) {
                            continue; // Skip pixel - sprite is behind wall
                        }
                    }

                    color_t* spriteTextureBuffer;
                    if (sprite.isAnimated) {
                        spriteTextureBuffer = assetStore->GetTexturePixelsAt(sprite.texture, sprite.col, sprite.row);
                    } else {
                        spriteTextureBuffer = assetStore->GetTexturePixels(sprite.texture);
                    }
                    if (spriteTextureBuffer != nullptr) {
                        color_t texelColor = spriteTextureBuffer[(textureWidth * (textureOffsetY)) + textureOffsetX];

                        // Skip transparent pixels (check alpha channel)
                        if ((texelColor & 0xFF000000) == 0) {
                            continue; // Skip transparent pixels
                        }

                        // Additional color-based transparency check (if needed)
                        // Some textures might use magenta (0xFF00FF) as transparency color
                        if ((texelColor & 0x00FFFFFF) == 0x00FF00FF) {
                            continue; // Skip magenta transparency color
                        }

                        drawPixel(colorBuffer, x, y, texelColor);
                    }
                }
            }
        }
    }





};

#endif