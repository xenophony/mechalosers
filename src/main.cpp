#include <SDL2/SDL.h>
#include "defs.h"
#include "Components/GridComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/TransformComponent.h"
#include <entt/entt.hpp>
#include "Systems/MapRenderSystem.h"
#include <glm/glm.hpp>
#include "Game/Game.h"
// #define color_t uint_32

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;
entt::registry registry;


const std::vector<std::vector<int>> grid = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}
};

bool initializeWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    int fullScreenWidth = display_mode.w;
    int fullScreenHeight = display_mode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullScreenWidth,
        fullScreenHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    colorBuffer = (color_t*)malloc(sizeof(color_t) * (color_t)WINDOW_WIDTH * (color_t)WINDOW_HEIGHT);

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    return true;
}

void destroyWindow() {
    free(colorBuffer);
    SDL_DestroyTexture(colorBufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void clearColorBuffer(color_t color) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            colorBuffer[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void renderColorBuffer() {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        (int)(WINDOW_WIDTH * sizeof(color_t))
    );
    SDL_RenderCopy(
        renderer,
        colorBufferTexture,
        NULL,
        NULL
    );

    SDL_RenderPresent(renderer);
}

// void drawPixel(int x, int y, color_t color) {
//     colorBuffer[(WINDOW_WIDTH * y) + x] = color;
// }

// void drawRect(int x, int y, int width, int height, color_t color) {
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             drawPixel(j + x, i + y, color);
//         }
//     }

// }

// void drawLine(int x0, int y0, int x1, int y1, color_t color) {
//     int deltaX = x1 - x0;
//     int deltaY = y1 - y0;

//     int side_length = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

//     float xInc = deltaX / (float)side_length;
//     float yInc = deltaY / (float)side_length;

//     float currentX = x0;
//     float currentY = y0;

//     for (int i = 0; i <= side_length; i++) {
//         drawPixel(round(currentX), round(currentY), color);
//         currentX += xInc;
//         currentY += yInc;
//     }
// }

void render() {
    // clearColorBuffer(0xFF000000);

    // renderWallProjection();
    // renderSpriteProjection();
    // mapRenderSystem->Update(registry, renderer, colorBuffer);

    // renderMapGrid();
    // renderMapRays();
    // renderMapPlayer();
    // renderMapSprites();

    // renderColorBuffer();
}

void processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            // Handle window close button
            exit(0);
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                // Handle escape key
                exit(0);
            }
            break;
        }
    }
}

int main() {
    // bool isGameRunning = initializeWindow();

    // if (!isGameRunning) {
    //     fprintf(stderr, "Failed to initialize window.\n");
    //     return 1;
    // }

    // Initialize the map render system after window initialization
    // mapRenderSystem = new MapRenderSystem();

    // Create an entity and add the grid component
    // entt::entity map = registry.create();
    // registry.emplace<GridComponent>(map, grid);

    // // Create Player
    // entt::entity player = registry.create();
    // registry.emplace<TransformComponent>(player, glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), glm::vec2(1.0, 1.0), 0.0);
    // registry.emplace<PlayerComponent>(player);
    // setup();

    // while (isGameRunning) {
    //     processInput();
    //     // update();
    //     render();

    //     // Add a small delay to prevent 100% CPU usage
    //     SDL_Delay(16); // ~60 FPS
    // }

    // destroyWindow();
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}