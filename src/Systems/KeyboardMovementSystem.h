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

        // Get the map component to determine dynamic boundaries
        GridComponent* mapComponent = nullptr;
        auto mapView = registry.view<GridComponent>();
        for (auto mapEntity : mapView) {
            mapComponent = &mapView.get<GridComponent>(mapEntity);
            break; // Get the first (and presumably only) map component
        }

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

                // Apply direct movement (frame-rate independent)
                rigidBody.velocity.x = inputDirection.x * playerSpeed;
                rigidBody.velocity.y = inputDirection.y * playerSpeed;
            }

            // Apply dynamic boundary checking based on actual map dimensions
            if (mapComponent && !mapComponent->grid.empty()) {
                // Calculate dynamic map boundaries from actual grid data
                const float MAP_HEIGHT = mapComponent->grid.size() * TILE_SIZE;
                const float MAP_WIDTH = mapComponent->grid[0].size() * TILE_SIZE;
                const float BOUNDARY_MARGIN = 32.0f; // Keep entities at least 32 units from map edge

                // Calculate potential new position
                glm::vec2 newPosition = transform.position + rigidBody.velocity * (float)deltaTime;

                // Clamp position to dynamic map boundaries
                if (newPosition.x < BOUNDARY_MARGIN) {
                    newPosition.x = BOUNDARY_MARGIN;
                    rigidBody.velocity.x = 0; // Stop horizontal movement
                }
                if (newPosition.x > MAP_WIDTH - BOUNDARY_MARGIN) {
                    newPosition.x = MAP_WIDTH - BOUNDARY_MARGIN;
                    rigidBody.velocity.x = 0; // Stop horizontal movement
                }
                if (newPosition.y < BOUNDARY_MARGIN) {
                    newPosition.y = BOUNDARY_MARGIN;
                    rigidBody.velocity.y = 0; // Stop vertical movement
                }
                if (newPosition.y > MAP_HEIGHT - BOUNDARY_MARGIN) {
                    newPosition.y = MAP_HEIGHT - BOUNDARY_MARGIN;
                    rigidBody.velocity.y = 0; // Stop vertical movement
                }

                // Update transform position with clamped values
                transform.position = newPosition;
            } else {
                // Fallback: If no map component found, just apply velocity without boundaries
                Logger::Log("Warning: No map component found for boundary checking");
                transform.position += rigidBody.velocity * (float)deltaTime;
            }
        }
    }
};

#endif
