#ifndef UTILS_H
#define UTILS_H


#include <math.h>
#include "defs.h"

void normalizeAngle(float* angle);

float distanceBetweenPoints(float x1, float y1, float x2, float y2);

int compareDescending(const void* a, const void* b);

#endif