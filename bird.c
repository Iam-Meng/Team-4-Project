#include "bird.h"
#include <math.h>

void initBird(Bird* bird, Tigr* sprite, float startX, float startY) {
    bird->x = startX;
    bird->y = startY;
    bird->velocity = 0.0f;
    bird->sprite = sprite;
    bird->alive = 1;
    bird->width = sprite->w;
    bird->height = sprite->h;
}

void updateBird(Bird* bird, float gravity, float groundLevel, float dt) {
    if (!bird->alive) return;
    
    // Apply gravity with time scaling
    bird->velocity += gravity * dt;
    bird->y += bird->velocity * dt;
    
    // Ground collision
    if (bird->y + bird->height > groundLevel) {
        bird->y = groundLevel - bird->height;
        bird->velocity = 0;
        bird->alive = 0;
    }
    
    // Ceiling collision
    if (bird->y < 0) {
        bird->y = 0;
        bird->velocity = 0;
    }
}

void jumpBird(Bird* bird, float jumpForce) {
    if (bird->alive) {
        bird->velocity = jumpForce;
    }
}

int checkCollision(Bird* bird, float pipeX, float pipeWidth, float gapY, float pipeGap) {
    if (!bird->alive) return 0;
    
    // Bird's collision box (slightly smaller than sprite)
    float birdLeft = bird->x + 5;
    float birdRight = bird->x + bird->width - 5;
    float birdTop = bird->y + 5;
    float birdBottom = bird->y + bird->height - 5;
    
    // Pipe's collision areas
    float pipeLeft = pipeX;
    float pipeRight = pipeX + pipeWidth;
    float gapTop = gapY - pipeGap/2;
    float gapBottom = gapY + pipeGap/2;
    
    // Check if bird is within pipe's x-range
    if (birdRight > pipeLeft && birdLeft < pipeRight) {
        // Check if bird is not in the gap
        if (birdTop < gapTop || birdBottom > gapBottom) {
            return 1;
        }
    }
    return 0;
}