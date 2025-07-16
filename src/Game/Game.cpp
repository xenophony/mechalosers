
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
#include "../Events/InputEvents.h"
#include "../Logger/Logger.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RaysComponent.h"
#include "../Components/SpriteComponent.h"

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
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();

    // Initialize systems
    mapRenderSystem = new MapRenderSystem();
    movementSystem = new MovementSystem();
    keyboardMovementSystem = new KeyboardMovementSystem();
    mouseLookSystem = new MouseLookSystem();
    rayCastingSystem = new RayCastingSystem();
    raysRenderSystem = new RaysRenderSystem();
    spriteRenderSystem = new SpriteRenderSystem();
    animationSystem = new AnimationSystem();
    Logger::Log("Game constructor called");
}



Game::~Game() {
    // Clean up systems
    delete mapRenderSystem;
    delete movementSystem;
    delete keyboardMovementSystem;
    delete mouseLookSystem;
    delete rayCastingSystem;
    delete raysRenderSystem;
    delete spriteRenderSystem;

    Logger::Log("Game destructor called");
}


void Game::Initialize() {
    Logger::Log("Starting SDL initialization");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        isRunning = false;
    }
    Logger::Log("SDL initialized successfully");
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
    SDL_SetRelativeMouseMode(SDL_TRUE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        isRunning = false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Logger::Log("Allocating color buffer");
    colorBuffer = (color_t*)malloc(sizeof(color_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    Logger::Log("Color buffer allocated successfully");

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    Logger::Log("Initialize method completed successfully");
    isRunning = true;
}

// void Game::Initialize() {


//     isRunning = true;
// }

// void Game::LoadLevel(int level) {

//     loader.LoadLevel(1);

// }

void Game::Setup() {
    Logger::Log("Starting Setup()");
    // Create a simplified level loader setup for testing the entt migration
    // LevelLoader loader;
    // lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
    // loader.LoadLevel(lua, registry, assetStore, renderer, 1);
        // Create an entity and add the grid component
    entt::entity map = registry.create();
    registry.emplace<GridComponent>(map, grid);
    Logger::Log("Grid component added");
    assetStore->loadTextures();
    Logger::Log("Textures loaded");

    // Create Player
    playerEntity = registry.create();
    registry.emplace<TransformComponent>(playerEntity, glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), glm::vec2(1.0, 1.0), 0.0);
    registry.emplace<RigidBodyComponent>(playerEntity, glm::vec2(0.0, 0.0));
    registry.emplace<PlayerComponent>(playerEntity);
    registry.emplace<KeyboardControlledComponent>(playerEntity);
    registry.emplace<MousePositionComponent>(playerEntity, glm::vec2(0.0, 0.0));
    registry.emplace<RaysComponent>(playerEntity);

    // Create a sample sprite entity with armor texture farther from player
    entt::entity armorSprite = registry.create();
    registry.emplace<TransformComponent>(armorSprite, glm::vec2(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2), glm::vec2(1.0, 1.0), 0.0);
    registry.emplace<SpriteComponent>(armorSprite, 13); // armor texture index

    entt::entity armorSprite2 = registry.create();
    registry.emplace<TransformComponent>(armorSprite2, glm::vec2(WINDOW_WIDTH / 2 + 300, WINDOW_HEIGHT / 2), glm::vec2(1.0, 1.0), 0.0);
    registry.emplace<SpriteComponent>(armorSprite2, 13); // armor texture index

    entt::entity trooper1 = registry.create();
    registry.emplace<TransformComponent>(trooper1, glm::vec2(200, 300), glm::vec2(1.0, 1.0), 0.0);
    registry.emplace<SpriteComponent>(trooper1, TROOPER, true);

    Logger::Log("Game setup complete with entt systems");

    keyboardMovementSystem->SubscribeToEvents(eventBus);


}

void Game::Run() {
    Logger::Log("Starting Game::Run()");
    Setup();
    Logger::Log("Setup completed, entering main loop");
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
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                eventBus->Emit<KeyPressedEvent>(KeyPressedEvent{ key });
            }
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
    // More precise frame timing
    static Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();

    // Calculate delta time in seconds with better precision
    double deltaTime = (currentTime - lastFrameTime) / 1000.0;
    lastFrameTime = currentTime;

    // Cap delta time to prevent large jumps (e.g., when debugging or alt-tabbing)
    if (deltaTime > 0.05) { // Cap at 50ms (20 FPS minimum)
        deltaTime = 0.05;
    }

    // Optional: Target 30 FPS with smoother frame pacing
    int timeToWait = FRAME_TIME_LENGTH - (currentTime - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        SDL_Delay(timeToWait);
    }

    millisecsPreviousFrame = currentTime;

    keyboardMovementSystem->Update(registry, deltaTime);
    mouseLookSystem->Update(registry, deltaTime);
    movementSystem->Update(registry, deltaTime);
    rayCastingSystem->Update(registry, deltaTime);
    animationSystem->Update(registry, deltaTime);

    // eventBus->Reset();
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
    raysRenderSystem->Render(registry, renderer, colorBuffer, assetStore);
    spriteRenderSystem->Render(registry, renderer, colorBuffer, assetStore, playerEntity);
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
