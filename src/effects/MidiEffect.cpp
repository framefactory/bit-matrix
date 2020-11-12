/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiEffect.h"

F_USE_NAMESPACE

MidiEffect::MidiEffect(const MidiMessage& message) :
    _message(message)
{
}

void MidiEffect::onRender(Bitmap* pBitmap, const Timing& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    uint8_t note = _message.note() % 64;
    int col = note / 8;
    int row = note % 8;

    uint32_t tic = timing.beat(16.0) % 14;
    uint32_t pos = tic < 8 ? tic : 14 - tic;
    pBitmap->fill(row * 8, col * 8 + pos, 8, 1);
}
