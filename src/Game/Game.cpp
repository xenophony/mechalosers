
#include "Game.h"

// #include "../Components/TransformComponent.h"
// #include "../Components/RigidBodyComponent.h"
// #include <SDL2/SDL.h>

// #include "../Components/GridComponent.h"
// #include "Components/RigidBodyComponent.h"
// #include "Components/PlayerComponent.h"
// #include "Components/TransformComponent.h"
// #include <entt/entt.hpp>
// #include "Systems/MapRenderSystem.h"
// #include <glm/glm.hpp>
#include "../Logger/Logger.h"

#include <iostream>
#include <string>
#include <sstream>



#include <glm/glm.hpp>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

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

Game::Game() {
    isRunning = false;
    isDebug = false;
    // assetStore = std::make_unique<AssetStore>();
    // eventBus = std::make_unique<EventBus>();

    // Initialize systems
    mapRenderSystem = new MapRenderSystem();

    Logger::Log("Game constructor called");
}



Game::~Game() {
    // Clean up systems
    delete mapRenderSystem;


    Logger::Log("Game destructor called");
}


void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        isRunning = false;
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
        isRunning = false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        isRunning = false;
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

    isRunning = true;
}

// void Game::Initialize() {


//     isRunning = true;
// }

// void Game::LoadLevel(int level) {

//     loader.LoadLevel(1);

// }

void Game::Setup() {
    // Create a simplified level loader setup for testing the entt migration
    // LevelLoader loader;
    // lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
    // loader.LoadLevel(lua, registry, assetStore, renderer, 1);
        // Create an entity and add the grid component
    entt::entity map = registry.create();
    registry.emplace<GridComponent>(map, grid);

    // Create Player
    entt::entity player = registry.create();
    registry.emplace<TransformComponent>(player, glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), glm::vec2(1.0, 1.0), 0.0);
    registry.emplace<PlayerComponent>(player);
    Logger::Log("Game setup complete with entt systems");
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }

}
void Game::ProcessInput() {
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

    // SDL_Event sdlEvent;
    // while (SDL_PollEvent(&sdlEvent)) {

    //     ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    //     ImGuiIO& io = ImGui::GetIO();

    //     int mouseX, mouseY;
    //     const int button = SDL_GetMouseState(&mouseX, &mouseY);

    //     io.MousePos = ImVec2(mouseX, mouseY);
    //     io.MouseDown[0] = button & SDL_BUTTON(SDL_BUTTON_LEFT);
    //     io.MouseDown[1] = button & SDL_BUTTON(SDL_BUTTON_RIGHT);

    //     switch (sdlEvent.type) {
    //     case SDL_QUIT:
    //         isRunning = false;
    //         break;
    //     case SDL_KEYDOWN:
    //         SDL_Keycode key = sdlEvent.key.keysym.sym;
    //         eventBus->EmitEvent<KeyPressedEvent>(key);
    //         if (key == SDLK_ESCAPE) {
    //             isRunning = false;
    //         }
    //         if (key == SDLK_p) {
    //             isDebug = !isDebug;
    //         }
    //         break;
    //     }
    // }

}
void Game::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();

    // eventBus->Reset();

    // Subscribe systems to events


    // Update systems with entt registry

}

void Game::ClearColorBuffer(color_t color) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            colorBuffer[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void Game::RenderColorBuffer() {
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

void Game::Render() {
    // SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    // SDL_RenderClear(renderer);

    // // Use entt render system
    // renderSystem->Update(registry, renderer, assetStore, camera);

    // // Additional render systems
    // renderTextSystem->Update(registry, renderer, assetStore, camera);
    // renderHealthSystem->Update(registry, renderer, camera);

    // if (isDebug) {
    //     renderCollisionSystem->Update(registry, renderer, camera);
    //     renderGUISystem->Update(registry, renderer, camera);
    // }

    // SDL_RenderPresent(renderer);
    ClearColorBuffer(0xFF000000);

    // renderWallProjection();
    // renderSpriteProjection();
    mapRenderSystem->Update(registry, renderer, colorBuffer);

    // renderMapGrid();
    // renderMapRays();
    // renderMapPlayer();
    // renderMapSprites();

    RenderColorBuffer();
}

void Game::Destroy() {
    // ImGui_ImplSDLRenderer2_Shutdown();
    // ImGui_ImplSDL2_Shutdown();
    // ImGui::DestroyContext();
    free(colorBuffer);
    SDL_DestroyTexture(colorBufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
