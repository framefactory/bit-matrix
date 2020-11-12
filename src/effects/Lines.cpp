/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Lines.h"
#include <cmath>

F_USE_NAMESPACE

void Lines::onRender(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();

    double f = fmod(timing.seconds * 0.25, 2.0);
    
    if (f < 1.0) {
        pBitmap->line(0, f * h, w, f * h);
    }
    else {
        f = f - 1.0;
        pBitmap->line(f * w, 0, f * w, h);
    }
}