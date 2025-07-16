#ifndef UTILS_H
#define UTILS_H

#include "../defs.h"
#include <vector>
#include <algorithm>

inline void drawPixel(color_t* colorBuffer, int x, int y, color_t color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        colorBuffer[(WINDOW_WIDTH * y) + x] = color;
    }
}

inline void drawRect(color_t* colorBuffer, int x, int y, int width, int height, color_t color) {
    // Clamp coordinates to valid ranges
    int startX = std::max(0, x);
    int startY = std::max(0, y);
    int endX = std::min(WINDOW_WIDTH, x + width);
    int endY = std::min(WINDOW_HEIGHT, y + height);

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            drawPixel(colorBuffer, j, i, color);
        }
    }
}

inline void drawLine(color_t* colorBuffer, int x0, int y0, int x1, int y1, color_t color) {
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int side_length = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    if (side_length == 0) {
        drawPixel(colorBuffer, x0, y0, color);
        return;
    }

    float xInc = deltaX / (float)side_length;
    float yInc = deltaY / (float)side_length;

    float currentX = x0;
    float currentY = y0;

    for (int i = 0; i <= side_length; i++) {
        drawPixel(colorBuffer, round(currentX), round(currentY), color);
        currentX += xInc;
        currentY += yInc;
    }
}

inline void normalizeAngle(float* angle) {
    *angle = remainder(*angle, TWO_PI);
    if (*angle < 0) {
        *angle = TWO_PI + *angle;
    }
}

inline float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

inline bool mapHasWallAt(GridComponent gridComponent, float x, float y) {
    std::vector<std::vector<int>> map = gridComponent.grid;
    int numCols = map.size();
    if (numCols == 0) return true;
    int numRows = map[0].size();

    if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE) {
        return true;
    }

    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);

    // Double-check bounds for array access
    if (mapGridIndexY < 0 || mapGridIndexY >= numCols || mapGridIndexX < 0 || mapGridIndexX >= numRows) {
        return true;
    }

    return map[mapGridIndexY][mapGridIndexX] != 0;
}

inline int getMapAt(GridComponent gridComponent, int i, int j)
{
    std::vector<std::vector<int>> map = gridComponent.grid;
    if (i < 0 || i >= map.size() || j < 0 || (map.size() > 0 && j >= map[0].size())) {
        return 1; // Return wall if out of bounds
    }
    return map[i][j];
}

inline bool isRayFacingDown(float angle) {
    return angle > 0 && angle < PI;
}
inline bool isRayFacingUp(float angle) {
    return !isRayFacingDown(angle);
}

inline bool isRayFacingRight(float angle) {
    return angle < 0.5 * PI || angle > 1.5 * PI;
}
inline bool isRayFacingLeft(float angle) {
    return !isRayFacingRight(angle);
}

inline bool isInsideMap(float x, float y) {
    return x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE;
}

#endif