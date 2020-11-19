/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_TEXTEFFECT_H
#define _ESP_BITMATRIX_TEXTEFFECT_H

#include "app.h"
#include "../MidiBeatEffect.h"

F_BEGIN_NAMESPACE

class TextEffect : public MidiBeatEffect
{
protected:
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_TEXTEFFECT_H