#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <math.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#define FOV_ANGLE (60 * (PI / 180))
#define NUM_RAYS WINDOW_WIDTH

#define TILE_SIZE 64

#define DIST_PROJ_PLANE  ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#define MINIMAP_SCALE_FACTOR 0.2

// #define FPS 30
#define FRAME_TIME_LENGTH (1000/ FPS)

#define PI 3.14159265
#define TWO_PI 6.28318530

typedef uint32_t color_t;

// TEXTURES 

#define RED_BRICK      0
#define PURPLE_STONE   1
#define MOSSY_STONE    2
#define GRAY_STONE     3
#define COLOR_STONE    4
#define BLUE_STONE     5
#define WOOD           6
#define EAGLE          7
#define PIKUMA         8
#define BARREL         9
#define LIGHT          10
#define TABLE          11
#define GUARD          12
#define ARMOR          13

#endif
