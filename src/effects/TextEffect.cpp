/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "TextEffect.h"
#include "../Fonts.h"

F_USE_NAMESPACE

void TextEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    //uint8_t note = message().note();
    //uint8_t vel = message().velocity();

    // TODO: Implement    
}
