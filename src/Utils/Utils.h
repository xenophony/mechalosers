#ifndef UTILS_H
#define UTILS_H

inline void drawPixel(color_t* colorBuffer, int x, int y, color_t color) {
    colorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

inline void drawRect(color_t* colorBuffer, int x, int y, int width, int height, color_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            drawPixel(colorBuffer, j + x, i + y, color);
        }
    }

}

#endif