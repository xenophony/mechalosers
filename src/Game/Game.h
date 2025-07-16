#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../defs.h"
#include "../Components/GridComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MousePositionComponent.h"
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include <entt/entt.hpp>
#include "../Systems/MapRenderSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/MouseLookSystem.h"
#include "../Systems/RayCastingSystem.h"
#include "../Systems/RaysRenderSystem.h"
#include "../AssetStore/AssetStore.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Systems/KeyboardMovementSystem.h"

class Game {
private:
    bool isRunning;
    bool isDebug;
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecsPreviousFrame = 0;
    color_t* colorBuffer = NULL;
    entt::registry registry;
    EventBus events;
    std::unique_ptr <AssetStore> assetStore;
    std::unique_ptr <EventBus> eventBus;
    SDL_Texture* colorBufferTexture;
    // sol::state lua;

    // EnTT-based systems
    MapRenderSystem* mapRenderSystem;
    KeyboardMovementSystem* keyboardMovementSystem;
    MovementSystem* movementSystem;
    MouseLookSystem* mouseLookSystem;
    RayCastingSystem* rayCastingSystem;
    RaysRenderSystem* raysRenderSystem;
    //...
    //...

public:
    Game();
    ~Game();
    void Initialize();
    void Setup();
    void Run();
    void ProcessInput();
    void Update();
    // void LoadLevel(int level);
    void Render();
    void RenderColorBuffer();
    void ClearColorBuffer(color_t color);
    void Destroy();
    std::vector<int> tilemap;
    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;

};

#endif