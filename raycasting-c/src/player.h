#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "defs.h"
#include "map.h"
#include "graphics.h"
#include "utils.h"

typedef struct {
    float x;
    float y;
    float width;
    float height;
    int turnDirection; //-1 for left, +1 for right
    int walkDirection; // -1 for back, +1 for front
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
} player_t;

extern player_t player;

void movePlayer(float deltaTime);

void renderMapPlayer();

#endif 
