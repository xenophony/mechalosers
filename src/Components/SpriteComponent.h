#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
    int width;
    int height;
    float distance;
    float angle;
    bool isFixed;
    bool isVisible;
    int texture;
    bool isAnimated;
    int col;
    int row;
    double lastFrame;
    SDL_RendererFlip flip;
    // std::string assetId;
    SDL_Rect srcRect;


    SpriteComponent(int texture, bool isAnimated = false, float distance = 0.0, float angle = 0.0, bool isVisible = false, bool isFixed = false, int srcRectX = 0, int srcRectY = 0, int height = 64, int width = 64) {
        this->texture = texture;
        this->width = width;
        this->height = height;
        this->distance = distance;
        this->angle = angle;
        this->isVisible = isVisible;
        this->flip = SDL_FLIP_NONE;
        this->isFixed = isFixed;
        this->isAnimated = isAnimated;
        this->srcRect = { srcRectX, srcRectY, width, height };
        this->col = 0;
        this->row = 0;
        this->lastFrame = 0;
    }
};

#endif