#ifndef TEXTURES_H
#define TEXTURES_H

#include "defs.h"
#include <stdint.h>
#include "upng.h"

#define NUM_TEXTURES 14

// typedef struct {
//     upng_t* upngTexture;
//     int width;
//     int height;
//     color_t* texture_buffer;
// } texture_t;

upng_t* textures[NUM_TEXTURES];

void loadTextures();
void freeTextures();

#endif