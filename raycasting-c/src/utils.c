#include "utils.h"

void normalizeAngle(float* angle){
    *angle = remainder(*angle, TWO_PI);
    if (*angle < 0) {
       *angle = TWO_PI + *angle;
    }
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int compareDescending(const void *a, const void *b) {
    return (*(int *)b - *(int *)a); // Reverse the comparison for descending order
}