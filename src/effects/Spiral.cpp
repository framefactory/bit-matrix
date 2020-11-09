/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Spiral.h"

F_USE_NAMESPACE

bool Spiral::onRender(const Timing& timing, Bitmap* pBitmap)
{
    int t = timing.seconds * 10.0;
    return true;
}