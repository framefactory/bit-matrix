/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_ANIMATIONEFFECT_H
#define _ESP_BITMATRIX_ANIMATIONEFFECT_H

#include "../app.h"
#include "../MidiBeatEffect.h"

#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class AnimationEffect : public MidiBeatEffect
{
protected:
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;

private:
    void animateCell(Bitmap* pBitmap, const MidiTiming& timing, uint8_t index, uint8_t pattern);
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_ANIMATIONEFFECT_H