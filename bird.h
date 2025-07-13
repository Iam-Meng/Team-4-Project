#ifndef BIRD_H
#define BIRD_H

#include "tigr.h"

typedef struct Bird {
    float x, y;
    float velocity;
    Tigr* sprite;
    int alive;
    int width, height;
} Bird;

void initBird(Bird* bird, Tigr* sprite, float startX, float startY);
void updateBird(Bird* bird, float gravity, float groundLevel, float dt);
void jumpBird(Bird* bird, float jumpForce);
int checkCollision(Bird* bird, float pipeX, float pipeWidth, float gapY, float pipeGap);

#endif