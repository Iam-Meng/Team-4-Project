#include "obstacle.h"
#include "tigr.h"

void draw_pipe(Tigr* screen, float x, float gapY, float pipeWidth, float pipeGap, int groundLevel) {
    // Top pipe (from top to gap)
    tigrFill(screen, x, 0, pipeWidth, 
             gapY - pipeGap/2, tigrRGB(60, 180, 70));
    
    // Pipe cap (top)
    tigrFill(screen, x - 5, gapY - pipeGap/2 - 20, 
             pipeWidth + 10, 20, tigrRGB(50, 160, 60));
    
    // Bottom pipe (from gap to bottom)
    tigrFill(screen, x, gapY + pipeGap/2, 
             pipeWidth, groundLevel - (gapY + pipeGap/2), tigrRGB(60, 180, 70));
    
    // Pipe cap (bottom)
    tigrFill(screen, x - 5, gapY + pipeGap/2, 
             pipeWidth + 10, 20, tigrRGB(50, 160, 60));
}