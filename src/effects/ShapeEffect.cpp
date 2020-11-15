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
    
    int row = note / 8;
    int col = note % 8;

    char text[2] = { vel, 0 };
    pBitmap->drawText(text, &Fonts::fontShapes1, col * 8, row * 8);
}
