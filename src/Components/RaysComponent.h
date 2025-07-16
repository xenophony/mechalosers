#ifndef RAYSCOMPONENT_H
#define RAYSCOMPONENT_H

#include "../defs.h"

struct Ray {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    bool wasHitVertical;
    int texture;
};

struct RaysComponent {
    std::vector<Ray> rays;

    RaysComponent() {
        rays.resize(NUM_RAYS); // Initialize with proper size
    };
};

#endif