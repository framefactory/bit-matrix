/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Index.h"
#include "fonts/Fonts.h"

F_USE_NAMESPACE

bool Index::onRender(const Timing& timing, Bitmap* pBitmap)
{
    int w = pBitmap->width();
    int h = pBitmap->height();
    int cols = w / 8;
    int rows = h / 8;

    int i = 0;
    int iInv = int(timing.seconds * 8.0) % (cols * rows);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c, ++i) {
            char text[8];
            sprintf(text, "%02d", i);
            pBitmap->drawText(text, &Fonts::font04B24, c * 8, r * 8, 4);

            if (i == iInv) {
                pBitmap->fill(c * 8, r * 8, 8, 8, Bitmap::Invert);
            }
        }
    }

    return true;
}