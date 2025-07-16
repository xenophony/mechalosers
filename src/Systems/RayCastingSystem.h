#ifndef RAYCASTINGSYSTEM_H
#define RAYCASTINGSYSTEM_H

#include "../defs.h"
#include "../Components/RaysComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GridComponent.h"
#include "../Utils/Utils.h"
#include <entt/entt.hpp>
#include <cmath>

class RayCastingSystem {
    void castRay(RaysComponent& rays, TransformComponent& transform, GridComponent& map, float rayAngle, int stripId) {
        normalizeAngle(&rayAngle);

        // Avoid division by zero for critical angles
        const float EPSILON = 1e-6f;
        if (fabs(sin(rayAngle)) < EPSILON) {
            rayAngle += EPSILON; // Slightly adjust angle to avoid division by zero
        }
        if (fabs(cos(rayAngle)) < EPSILON) {
            rayAngle += EPSILON; // Slightly adjust angle to avoid division by zero
        }

        float xintercept, yintercept;
        float xstep, ystep;

        ///////////////////////////////////////////
        // HORIZONTAL RAY-GRID INTERSECTION CODE
        ///////////////////////////////////////////
        bool foundHorzWallHit = false;
        float horzWallHitX = 0;
        float horzWallHitY = 0;
        int horzWallTexture = 0;

        // Find the y-coordinate of the closest horizontal grid intersection
        yintercept = floor(transform.position.y / TILE_SIZE) * TILE_SIZE;
        yintercept += isRayFacingDown(rayAngle) ? TILE_SIZE : 0;

        // Find the x-coordinate of the closest horizontal grid intersection
        float tanRayAngle = tan(rayAngle);
        if (fabs(tanRayAngle) < EPSILON) {
            tanRayAngle = EPSILON; // Prevent division by zero
        }
        xintercept = transform.position.x + (yintercept - transform.position.y) / tanRayAngle;

        // Calculate the increment xstep and ystep
        ystep = TILE_SIZE;
        ystep *= isRayFacingUp(rayAngle) ? -1 : 1;

        xstep = TILE_SIZE / tanRayAngle;
        xstep *= (isRayFacingLeft(rayAngle) && xstep > 0) ? -1 : 1;
        xstep *= (isRayFacingRight(rayAngle) && xstep < 0) ? -1 : 1;

        float nextHorzTouchX = xintercept;
        float nextHorzTouchY = yintercept;

        // Increment xstep and ystep until we find a wall (with max iterations to prevent infinite loops)
        int maxIterations = 100;
        int iterations = 0;
        while (isInsideMap(map, nextHorzTouchX, nextHorzTouchY) && iterations < maxIterations) {
            iterations++;
            float xToCheck = nextHorzTouchX;
            float yToCheck = nextHorzTouchY + (isRayFacingUp(rayAngle) ? -1 : 0);

            if (mapHasWallAt(map, xToCheck, yToCheck)) {
                // found a wall hit
                horzWallHitX = nextHorzTouchX;
                horzWallHitY = nextHorzTouchY;
                horzWallTexture = getMapAt(map, (int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
                foundHorzWallHit = true;
                break;
            } else {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }

        ///////////////////////////////////////////
        // VERTICAL RAY-GRID INTERSECTION CODE
        ///////////////////////////////////////////
        int foundVertWallHit = false;
        float vertWallHitX = 0;
        float vertWallHitY = 0;
        int vertWallTexture = 0;

        // Find the x-coordinate of the closest vertical grid intersection
        xintercept = floor(transform.position.x / TILE_SIZE) * TILE_SIZE;
        xintercept += isRayFacingRight(rayAngle) ? TILE_SIZE : 0;

        // Find the y-coordinate of the closest vertical grid intersection
        yintercept = transform.position.y + (xintercept - transform.position.x) * tanRayAngle;

        // Calculate the increment xstep and ystep
        xstep = TILE_SIZE;
        xstep *= isRayFacingLeft(rayAngle) ? -1 : 1;

        ystep = TILE_SIZE * tanRayAngle;
        ystep *= (isRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
        ystep *= (isRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;

        float nextVertTouchX = xintercept;
        float nextVertTouchY = yintercept;

        // Increment xstep and ystep until we find a wall (with max iterations to prevent infinite loops)
        maxIterations = 100;
        iterations = 0;
        while (isInsideMap(map, nextVertTouchX, nextVertTouchY) && iterations < maxIterations) {
            iterations++;
            float xToCheck = nextVertTouchX + (isRayFacingLeft(rayAngle) ? -1 : 0);
            float yToCheck = nextVertTouchY;

            if (mapHasWallAt(map, xToCheck, yToCheck)) {
                // found a wall hit
                vertWallHitX = nextVertTouchX;
                vertWallHitY = nextVertTouchY;
                vertWallTexture = getMapAt(map, (int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
                foundVertWallHit = true;
                break;
            } else {
                nextVertTouchX += xstep;
                nextVertTouchY += ystep;
            }
        }

        // Calculate both horizontal and vertical hit distances and choose the smallest one
        float horzHitDistance = foundHorzWallHit
            ? distanceBetweenPoints(transform.position.x, transform.position.y, horzWallHitX, horzWallHitY)
            : FLT_MAX;
        float vertHitDistance = foundVertWallHit
            ? distanceBetweenPoints(transform.position.x, transform.position.y, vertWallHitX, vertWallHitY)
            : FLT_MAX;

        if (vertHitDistance < horzHitDistance) {
            rays.rays[stripId].distance = vertHitDistance;
            rays.rays[stripId].wallHitX = vertWallHitX;
            rays.rays[stripId].wallHitY = vertWallHitY;
            rays.rays[stripId].texture = vertWallTexture;
            rays.rays[stripId].wasHitVertical = true;
            rays.rays[stripId].rayAngle = rayAngle;
        } else {
            rays.rays[stripId].distance = horzHitDistance;
            rays.rays[stripId].wallHitX = horzWallHitX;
            rays.rays[stripId].wallHitY = horzWallHitY;
            rays.rays[stripId].texture = horzWallTexture;
            rays.rays[stripId].wasHitVertical = false;
            rays.rays[stripId].rayAngle = rayAngle;
        }

    }

public:
    void Update(entt::registry& registry, double deltaTime) {
        auto entities = registry.view<RaysComponent, TransformComponent>();
        // float rayAngle = player.rotationAngle - (FOV_ANGLE /2);
        GridComponent map;
        auto theMap = registry.view<GridComponent>();
        for (auto grid : theMap) {
            map = theMap.get<GridComponent>(grid);
        }



        for (auto entity : entities) {
            for (int col = 0; col < NUM_RAYS; col++) {
                auto& transform = entities.get<TransformComponent>(entity);
                auto& rays = entities.get<RaysComponent>(entity);

                // Calculate ray angle accounting for wider rays
                // Each ray now represents RAY_WIDTH screen columns
                float screenColumn = col * RAY_WIDTH + (RAY_WIDTH / 2.0f); // Center of the ray strip
                float rayAngle = transform.rotation + atan((screenColumn - WINDOW_WIDTH / 2) / DIST_PROJ_PLANE);

                castRay(rays, transform, map, rayAngle, col);
            }
        }


    }
};

#endif