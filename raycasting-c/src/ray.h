#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "utils.h"

typedef struct {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    bool wasHitVertical;
    int texture;
} ray_t;

extern ray_t rays[NUM_RAYS];

// void normalizeAngle(float* angle);
// float distanceBetweenPoints(float x1, float y1, float x2, float y2);

bool isRayFacingUp(float angle);
bool isRayFacingDown(float angle);
bool isRayFacingLeft(float angle);
bool isRayFacingRight(float angle);

void castRay(float rayAngle, int stripId);
void castAllRays();
void renderMapRays();

#endif
