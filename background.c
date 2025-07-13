#include "background.h"
#include <stddef.h>

Tigr* load_scaled_background(const char* path, int screenWidth, int screenHeight) {
    Tigr* original = tigrLoadImage(path);
    if (!original) return NULL;
    
    Tigr* scaled = tigrBitmap(screenWidth, screenHeight);
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            int srcX = (int)((float)x / screenWidth * original->w);
            int srcY = (int)((float)y / screenHeight * original->h);
            
            srcX = srcX < 0 ? 0 : (srcX >= original->w ? original->w - 1 : srcX);
            srcY = srcY < 0 ? 0 : (srcY >= original->h ? original->h - 1 : srcY);
            
            TPixel pixel = tigrGet(original, srcX, srcY);
            tigrPlot(scaled, x, y, pixel);
        }
    }
    tigrFree(original);
    return scaled;
}