/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_SHAPEEFFECT_H
#define _ESP_BITMATRIX_SHAPEEFFECT_H

#include "../app.h"
#include "../MidiBeatEffect.h"

#include "net/MidiMessage.h"
#include "core/Random.h"

F_BEGIN_NAMESPACE

class ShapeEffect : public MidiBeatEffect
{
protected:
    void onStart(const MidiTiming& timing, const MidiMessage& message) override;
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;

    void onBeat(uint32_t index, uint32_t count) override;

private:
    Random _generator;
    uint32_t _random[4];
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_SHAPEEFFECT_H