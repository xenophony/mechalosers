#ifndef MAPRENDERSYSTEM_H
#define MAPRENDERSYSTEM_H

#include <entt/entt.hpp>
// #include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include "../Components/GridComponent.h"
#include "../Components/PlayerComponent.h"
#include <SDL2/SDL.h>
#include "../defs.h"
#include "../Utils/Utils.h"

class MapRenderSystem {
public:
    void Update(entt::registry& registry, SDL_Renderer* renderer, color_t* colorBuffer) {


        auto view = registry.view<GridComponent>();

        for (auto entity : view) {
            const auto& grid = view.get<GridComponent>(entity);

            for (int i = 0; i < grid.grid.size(); ++i) { // Iterate through rows
                for (int j = 0; j < grid.grid[i].size(); ++j) {
                    int tileX = j * TILE_SIZE;
                    int tileY = i * TILE_SIZE;
                    color_t tileColor = grid.grid[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;

                    drawRect(
                        colorBuffer,
                        floor(tileX * MINIMAP_SCALE_FACTOR),
                        floor(tileY * MINIMAP_SCALE_FACTOR),
                        ceil(TILE_SIZE * MINIMAP_SCALE_FACTOR),
                        ceil(TILE_SIZE * MINIMAP_SCALE_FACTOR),
                        tileColor
                    );

                }
            }
        }

        auto thePlayer = registry.view<PlayerComponent, TransformComponent>();

        for (auto player : thePlayer) {
            const auto& transform = thePlayer.get<TransformComponent>(player);
            drawRect(
                colorBuffer,
                transform.position.x * MINIMAP_SCALE_FACTOR,
                transform.position.y * MINIMAP_SCALE_FACTOR,
                5 * MINIMAP_SCALE_FACTOR,
                5 * MINIMAP_SCALE_FACTOR,
                0xFFFFFFFF
            );
        }

    }

};
#endif
