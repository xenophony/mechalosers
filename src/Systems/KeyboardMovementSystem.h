#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GridComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/InputEvents.h"
#include "../Logger/Logger.h"
#include "../defs.h"
#include <string>
#include <cmath>
#include <algorithm>

class KeyboardMovementSystem {
private:
    float playerSpeed;

public:
    KeyboardMovementSystem() : playerSpeed(720.0f) {
        Logger::Log("KeyboardMovementSystem constructor: playerSpeed set to " + std::to_string(playerSpeed));
    }

    void OnKeyPressed(const KeyPressedEvent& event) {
        // Event handling logic can go here if needed
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->Subscribe<KeyPressedEvent, &KeyboardMovementSystem::OnKeyPressed>(*this);
    }

    void Update(entt::registry& registry, double deltaTime) {
        // Get current keyboard state
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        // Get all entities that have KeyboardControlledComponent and RigidBodyComponent
        auto view = registry.view<KeyboardControlledComponent, RigidBodyComponent, TransformComponent>();

        for (auto entity : view) {
            auto& keyboardController = view.get<KeyboardControlledComponent>(entity);
            auto& rigidBody = view.get<RigidBodyComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);

            // Reset velocity every frame (direct control)
            rigidBody.velocity = glm::vec2(0, 0);

            // Accumulate movement input from multiple keys
            glm::vec2 inputDirection(0, 0);

            // Check for key presses and accumulate movement directions
            if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
                inputDirection.x += cos(transform.rotation);
                inputDirection.y += sin(transform.rotation);
            }
            if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
                inputDirection.x += -sin(transform.rotation);
                inputDirection.y += cos(transform.rotation);
            }
            if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
                inputDirection.x += -cos(transform.rotation);
                inputDirection.y += -sin(transform.rotation);
            }
            if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
                inputDirection.x += sin(transform.rotation);
                inputDirection.y += -cos(transform.rotation);
            }

            // Normalize diagonal movement to prevent faster movement
            if (inputDirection.x != 0 || inputDirection.y != 0) {
                float magnitude = sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
                inputDirection.x /= magnitude;
                inputDirection.y /= magnitude;

                // Set velocity - let MovementSystem handle position updates and collision detection
                rigidBody.velocity.x = inputDirection.x * playerSpeed;
                rigidBody.velocity.y = inputDirection.y * playerSpeed;
            }
        }
    }
};

#endif
