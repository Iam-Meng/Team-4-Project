#include "background.h"

Tigr* scaleImage(Tigr* src, int newWidth, int newHeight) {
    Tigr* dest = tigrBitmap(newWidth, newHeight);
    if (!dest) return NULL;
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)((float)x / newWidth * src->w);
            int srcY = (int)((float)y / newHeight * src->h);
            if (srcX < 0) srcX = 0;
            if (srcX >= src->w) srcX = src->w - 1;
            if (srcY < 0) srcY = 0;
            if (srcY >= src->h) srcY = src->h - 1;
            dest->pix[y * dest->w + x] = src->pix[srcY * src->w + srcX];
        }
    }
    return dest;
}

