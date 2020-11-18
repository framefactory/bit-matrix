/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_BIGEFFECT_H
#define _ESP_BITMATRIX_BIGEFFECT_H

#include "../app.h"
#include "../MidiEffect.h"

#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class BigEffect : public MidiEffect
{
public:
    BigEffect(const MidiMessage& message) :
        MidiEffect(message) { }

protected:
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_BIGEFFECT_H