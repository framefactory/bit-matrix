/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_STATICKEYEFFECT_H
#define _ESP_BITMATRIX_STATICKEYEFFECT_H

#include "../app.h"

#include "effect/Effect.h"
#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class StaticKeyEffect : public Effect
{
public:
    StaticKeyEffect(const MidiMessage& message);

protected:
    void onRender(Bitmap* pBitmap, const Timing& timing) override;
    double random();

private:
    MidiMessage _message;
    int _b8;
    int _b16;
    int _b32;
    double _r8;
    double _r16;
    double _r32;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_STATICKEYEFFECT_H