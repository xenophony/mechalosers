#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../defs.h"
#include "../Components/GridComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <entt/entt.hpp>
#include "../Systems/MapRenderSystem.h"
#include <glm/glm.hpp>
#include <vector>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    bool isDebug;
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecsPreviousFrame = 0;
    color_t* colorBuffer = NULL;
    entt::registry registry;
    // std::unique_ptr <AssetStore> assetStore;
    // std::unique_ptr <EventBus> eventBus;
    SDL_Texture* colorBufferTexture;
    // sol::state lua;

    // EnTT-based systems
    class MapRenderSystem* mapRenderSystem;
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