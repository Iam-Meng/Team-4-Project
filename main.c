#include <stdio.h>
#include <math.h>
#define TIGR_IMPLEMENTATION
#include "tigr.h"
#include "background.h"

// Custom image scaling function using nearest-neighbor interpolation
Tigr* scaleImage(Tigr* src, int newWidth, int newHeight) {
    Tigr* dest = tigrBitmap(newWidth, newHeight);
    if (!dest) return NULL;
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            // Calculate corresponding position in original image
            int srcX = (int)((float)x / newWidth * src->w);
            int srcY = (int)((float)y / newHeight * src->h);
            
            // Clamp coordinates to valid range
            if (srcX < 0) srcX = 0;
            if (srcX >= src->w) srcX = src->w - 1;
            if (srcY < 0) srcY = 0;
            if (srcY >= src->h) srcY = src->h - 1;
            
            // Copy pixel directly using pixel array
            dest->pix[y * dest->w + x] = src->pix[srcY * src->w + srcX];
        }
    }
    return dest;
}

int main() {
    int screenWidth = 680, screenHeight = 720;
    Tigr* screen = tigrWindow(screenWidth, screenHeight, "Background Image Loader", TIGR_FIXED);
    if (!screen) return 1;

    const char* imagePath = "background2.png";
    Tigr* original = tigrLoadImage(imagePath);
    Tigr* background = NULL;

    if (original) {
        float scale = fminf((float)screenWidth / original->w, (float)screenHeight / original->h);
        int w = original->w * scale, h = original->h * scale;
        background = scaleImage(original, w, h);
        tigrFree(original);
    }
    if (!background) {
        background = tigrBitmap(screenWidth, screenHeight);
        tigrClear(background, tigrRGB(30, 20, 40));
        tigrPrint(background, tfont, 50, screenHeight/2 - 30, tigrRGB(255,180,180), "ERROR LOADING BACKGROUND");
    }

    int bgX = (screenWidth - background->w) / 2, bgY = (screenHeight - background->h) / 2;
    Tigr* frame = tigrBitmap(background->w + 10, background->h + 10);
    tigrClear(frame, tigrRGB(60, 60, 80));
    tigrBlit(frame, background, 5, 5, 0, 0, background->w, background->h);

    while (!tigrClosed(screen)) {
        if (tigrKeyDown(screen, TK_ESCAPE)) break;
        tigrClear(screen, tigrRGB(20, 25, 35));
        tigrBlit(screen, frame, bgX - 5, bgY - 5, 0, 0, frame->w, frame->h);
        tigrUpdate(screen);
    }

    tigrFree(background);
    tigrFree(frame);
    tigrFree(screen);
    return 0;
}