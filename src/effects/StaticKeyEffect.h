/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_STATICKEYEFFECT_H
#define _ESP_BITMATRIX_STATICKEYEFFECT_H

#include "../app.h"
#include "../MidiEffect.h"

#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class StaticKeyEffect : public MidiEffect
{
public:
    StaticKeyEffect(const MidiMessage& message) :
        MidiEffect(message) {}
        
protected:
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;
    double random();

private:
    int _b8;
    int _b16;
    int _b32;
    double _r8;
    double _r16;
    double _r32;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_STATICKEYEFFECT_H