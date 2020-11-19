/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_MIDIBEATEFFECT_H
#define _ESP_BITMATRIX_MIDIBEATEFFECT_H

#include "app.h"
#include "MidiEffect.h"

F_BEGIN_NAMESPACE

class MidiBeatEffect : public MidiEffect
{
protected:
    void dispatchBeats(const MidiTiming& timing);

    virtual void onBeat(uint32_t index, uint32_t count) {};

private:
    uint32_t _count[4] { 0, 0, 0, 0 };
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_MIDIBEATEFFECT_H