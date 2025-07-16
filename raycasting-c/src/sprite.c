#include "sprite.h"
#include "utils.h"
#include "ray.h"

#define NUM_SPRITES 4

static sprite_t sprites[NUM_SPRITES] = {
    { .x = 640, .y = 630, .texture= 9},
    { .x = 590, .y = 630, .texture= 9},
    { .x = 100, .y = 700, .texture= ARMOR},
    { .x = 200, .y = 600, .texture= TABLE}
};

int compareSprites(const void* a, const void* b) {
    const sprite_t* spriteA = (const sprite_t*)a;
    const sprite_t* spriteB = (const sprite_t*)b;

    if (spriteA->distance < spriteB->distance) return 1;
    if (spriteA->distance > spriteB->distance) return -1;
    return 0;
}

void renderSpriteProjection(void){
    sprite_t visibleSprites[NUM_SPRITES];
    int numVisibleSprites = 0;

   
    
    for (int i = 0; i < NUM_SPRITES; i++) {
        float angleSpritePlayer = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);
    
        if(angleSpritePlayer > PI){
            angleSpritePlayer -= TWO_PI;
        }
        if(angleSpritePlayer < -PI){
            angleSpritePlayer += TWO_PI;
        }
        angleSpritePlayer= fabs(angleSpritePlayer);
        
        const float EPSILON = 0.1;
        if(angleSpritePlayer < (FOV_ANGLE/2) + EPSILON){
            sprites[i].visible = true;
            sprites[i].angle = angleSpritePlayer;
            sprites[i].distance = distanceBetweenPoints(
                sprites[i].x, 
                sprites[i].y, 
                player.x, 
                player.y
            );     
            visibleSprites[numVisibleSprites] = sprites[i];
            numVisibleSprites++;
        } else {
            sprites[i].visible = false;
        }
    }

    qsort(visibleSprites, numVisibleSprites, sizeof(sprite_t), compareSprites);

    for (int i = 0; i < numVisibleSprites; i++) {
        sprite_t sprite = visibleSprites[i];

        float perpDistance = sprite.distance * cos(sprite.angle);

        float spriteHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
        float spriteWidth = spriteHeight;

        float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
        spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

        float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
        spriteBottomY = (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;
            // int spriteMidX = DIST_PROJ_PLANE * ((sprite.x - player.x)/ (sprite.y - player.y));
            // int spriteStartX = spriteMidx - (spriteWidth/2);
            // int spriteEndX = spriteMidX + (spriteWidth/2);

        float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotationAngle;
        float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

        float spriteLeftX = (WINDOW_WIDTH/2) + spriteScreenPosX - (spriteWidth / 2);
        float spriteRightX = spriteLeftX + spriteWidth;

        int textureWidth = upng_get_width(textures[sprite.texture]);
        int textureHeight = upng_get_height(textures[sprite.texture]);

        for (int x = spriteLeftX; x < spriteRightX; x++) {
            float texelWidth = (textureWidth / spriteWidth);
            int textureOffsetX = (x - spriteLeftX) * texelWidth;
            for (int y = spriteTopY; y < spriteBottomY; y++) {
                int distanceFromTop = y + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
                float texelHeight = (textureHeight / spriteHeight);
                int textureOffsetY = distanceFromTop * texelHeight;
                if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT){
                    color_t* spriteTextureBuffer = (color_t*)upng_get_buffer(textures[sprite.texture]);
                    color_t texelColor = spriteTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];
                    if(sprite.distance < rays[x].distance && texelColor != 0xFFFF00FF){
                        // printf("Value: %" PRIu32 "\n", texelColor);
                        drawPixel(x, y, texelColor);
                    }
                    
                }
                    
            }
        }
    }
}

void renderMapSprites(void){
    for (int i = 0; i < NUM_SPRITES; i++){
        drawRect(
            sprites[i].x * MINIMAP_SCALE_FACTOR, 
            sprites[i].y * MINIMAP_SCALE_FACTOR, 
            2, 
            2, 
            (sprites[i].visible ? 0xFF00FFFF : 0xFF444444)
        );
    }
}

