#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "tigr.h"

// Scales a Tigr image to new dimensions using nearest-neighbor interpolation
Tigr* scaleImage(Tigr* src, int newWidth, int newHeight);

#endif // BACKGROUND_H