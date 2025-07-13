#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "tigr.h"

typedef struct Pipe {
    float x;
    float gapY;
    int passed;
} Pipe;

void draw_pipe(Tigr* screen, float x, float gapY, float pipeWidth, float pipeGap, int groundLevel);

#endif