/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_MIDIEFFECT_H
#define _ESP_BITMATRIX_MIDIEFFECT_H

#include "../app.h"

#include "effect/Effect.h"
#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class MidiEffect : public Effect
{
public:
    MidiEffect(const MidiMessage& message);

protected:
    void onRender(Bitmap* pBitmap, const Timing& timing) override;

private:
    MidiMessage _message;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_MIDIEFFECT_H