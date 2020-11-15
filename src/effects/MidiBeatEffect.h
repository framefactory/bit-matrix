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
    void dispatchBeats(const Timing& timing);

    virtual void onB1(uint32_t count) {};
    virtual void onB2(uint32_t count) {};
    virtual void onB4(uint32_t count) {};
    virtual void onB8(uint32_t count) {};
    virtual void onB16(uint32_t count) {};
    virtual void onB32(uint32_t count) {};

private:
    uint32_t _b1count = 0;
    uint32_t _b2count = 0;
    uint32_t _b4count = 0;
    uint32_t _b8count = 0;
    uint32_t _b16count = 0;
    uint32_t _b32count = 0;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_MIDIBEATEFFECT_H