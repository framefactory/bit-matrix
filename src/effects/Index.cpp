/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Index.h"
#include "fonts/Fonts.h"

F_USE_NAMESPACE

Index::Index(int offsetX, int offsetY, int strideX, int strideY) :
    _offsetX(offsetX),
    _offsetY(offsetY),
    _strideX(strideX),
    _strideY(strideY)
{
}

void Index::onRender(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();
    int cols = w / _strideX;
    int rows = h / _strideY;

    int i = 0;
    int iInv = int(timing.seconds * 4.0) % (cols * rows);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c, ++i) {
            char text[8];
            sprintf(text, "%02d", i);
            pBitmap->drawText(text, &Fonts::font04B24, _offsetX + c * _strideX, _offsetY + r * _strideY, 4);

            if (i == iInv) {
                pBitmap->fill(_offsetX + c * _strideX, _offsetY + r * _strideY, 8, 8, Bitmap::Invert);
            }
        }
    }
}