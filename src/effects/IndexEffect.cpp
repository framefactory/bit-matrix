/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "IndexEffect.h"
#include "fonts/Fonts.h"

F_USE_NAMESPACE

IndexEffect::IndexEffect(int offsetX, int offsetY, int strideX, int strideY) :
    _offsetX(offsetX),
    _offsetY(offsetY),
    _strideX(strideX),
    _strideY(strideY)
{
}

void IndexEffect::onRender(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();
    int cols = w / _strideX;
    int rows = h / _strideY;
    int size = cols * rows;

    int i = 0;
    int t = int(timing.seconds * 6.0);
    int inv1 = t % size;
    int inv2 = (t + 32) % size;
    int clear1 = (t + 16) % size;
    int clear2 = (t + 48) % size;
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c, ++i) {
            char text[8];
            sprintf(text, "%02d", i);
            pBitmap->drawText(text, &Fonts::font04B24, _offsetX + c * _strideX, _offsetY + r * _strideY, 4);

            if (i == inv1 || i == inv2) {
                pBitmap->fill(_offsetX + c * _strideX, _offsetY + r * _strideY, 8, 8, Bitmap::Invert);
            }
            if (i == clear1 || i == clear2) {
                pBitmap->fill(_offsetX + c * _strideX, _offsetY + r * _strideY, 8, 8, Bitmap::Clear);
            }
        }
    }
}