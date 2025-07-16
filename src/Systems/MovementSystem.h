#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
// #include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
// #include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"

class MovementSystem {
public:
    // void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
    //     eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
    // }

    // void OnCollision(CollisionEvent& event) {
    //     // For entt, we'll need to modify this to work with entity IDs
    //     // For now, let's comment out the collision handling and focus on basic movement
    //     /*
    //     Entity a = event.a;
    //     Entity b = event.b;

    //     if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
    //         Logger::Log("The Movement system received an event collision between entities " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
    //         OnEnemyHitsObstacle(b);
    //     }

    //     if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
    //         Logger::Log("The Movement system received an event collision between entities " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
    //         OnEnemyHitsObstacle(a);
    //     }
    //     */
    // }

    void Update(entt::registry& registry, double deltaTime) {
        // Get all entities that have both TransformComponent and RigidBodyComponent
        auto view = registry.view<TransformComponent, RigidBodyComponent>();

        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& rigidBody = view.get<RigidBodyComponent>(entity);

            // Update entity position based on its velocity
            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;

            rigidBody.velocity = glm::vec2(0, 0);
        }
    }

private:
    // void OnEnemyHitsObstacle(entt::entity enemy, entt::registry& registry) {
    //     if (registry.all_of<RigidBodyComponent, SpriteComponent>(enemy)) {
    //         auto& rigidbody = registry.get<RigidBodyComponent>(enemy);
    //         auto& sprite = registry.get<SpriteComponent>(enemy);

    //         rigidbody.velocity.x *= -1;
    //         rigidbody.velocity.y *= -1;

    //         if (rigidbody.velocity.x != 0) {
    //             sprite.flip = (rigidbody.velocity.x > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    //         }
    //     }
    // }
};

#endif
