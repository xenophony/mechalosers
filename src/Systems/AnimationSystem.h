#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GridComponent.h"
// #include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
// #include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"



class AnimationSystem {
public:


    void Update(entt::registry& registry, double deltaTime) {
        // Get the grid component (should be on the map entity)
        auto sprites = registry.view<SpriteComponent>();
        if (sprites.empty()) {
            return; // No grid found, can't do collision detection
        }

        int ANIMATION_CONSTANT = 1000;


        for (auto entity : sprites) {
            auto& sprite = sprites.get<SpriteComponent>(entity);
            if (sprite.isVisible && sprite.isAnimated) {
                if (sprite.lastFrame + ANIMATION_CONSTANT > deltaTime) {
                    sprite.row = ((sprite.row) % 4) + 1;
                    sprite.lastFrame = deltaTime;
                }
            }

        }
    }

private:

};

#endif
