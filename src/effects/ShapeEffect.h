/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_SHAPEEFFECT_H
#define _ESP_BITMATRIX_SHAPEEFFECT_H

#include "../app.h"

#include "MidiEffect.h"
#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class ShapeEffect : public MidiEffect
{
public:
    ShapeEffect(const MidiMessage& message) :
        MidiEffect(message) { }

protected:
    void onRender(Bitmap* pBitmap, const Timing& timing) override;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_SHAPEEFFECT_H