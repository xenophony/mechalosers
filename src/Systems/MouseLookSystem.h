#ifndef MOUSELOOKSYSTEM_H
#define MOUSELOOKSYSTEM_H

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MousePositionComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/InputEvents.h"
#include "../Logger/Logger.h"
#include "../defs.h"
#include <string>
#include <cmath>
#include <algorithm>

class MouseLookSystem {
private:
    float mouseSensitivity;

public:
    MouseLookSystem() : mouseSensitivity(0.005f) {
        Logger::Log("MouseLookSystem constructor: mouseSensitivity set to " + std::to_string(mouseSensitivity));
    }

    void Update(entt::registry& registry, double deltaTime) {
        // Get all entities that have MousePositionComponent and TransformComponent
        auto view = registry.view<MousePositionComponent, TransformComponent>();

        for (auto entity : view) {
            auto& mousePosition = view.get<MousePositionComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);

            // Get relative mouse movement
            int deltaX, deltaY;
            SDL_GetRelativeMouseState(&deltaX, &deltaY);

            // Store current position as last position
            mousePosition.lastPosition = mousePosition.position;

            // Update current mouse position (convert int to float)
            mousePosition.position.x = static_cast<float>(deltaX);
            mousePosition.position.y = static_cast<float>(deltaY);

            // Apply direct mouse movement (simple and responsive)
            if (deltaX != 0) {
                transform.rotation += deltaX * mouseSensitivity;

                // Keep rotation in range [0, 2π]
                while (transform.rotation >= 2 * M_PI) {
                    transform.rotation -= 2 * M_PI;
                }
                while (transform.rotation < 0) {
                    transform.rotation += 2 * M_PI;
                }

                // Logger::Log("Mouse moved: deltaX=" + std::to_string(deltaX) +
                //     ", new rotation=" + std::to_string(transform.rotation));
            }
        }
    }
};

#endif
