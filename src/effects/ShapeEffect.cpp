/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ShapeEffect.h"
#include "../Fonts.h"

F_USE_NAMESPACE

void ShapeEffect::onRender(Bitmap* pBitmap, const Timing& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    uint8_t note = message().note();
    uint8_t vel = message().velocity();
    
    int row = vel / 8;
    int col = vel % 8;

    uint32_t tic = timing.beat(16.0) % 14;
    uint32_t pos = tic < 8 ? tic : 14 - tic;
    pBitmap->fill(row * 8, col * 8 + pos, 8, 1);
}
