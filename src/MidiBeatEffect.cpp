/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiBeatEffect.h"

F_USE_NAMESPACE

MidiBeatEffect::MidiBeatEffect(const MidiMessage& message) :
    MidiEffect(message),
    _count{ 0, 0, 0, 0 }
{
}

void MidiBeatEffect::dispatchBeats(const MidiTiming& timing)
{
    uint32_t count[8] {
        timing.beat(4.0),
        timing.beat(8.0),
        timing.beat(16.0),
        timing.beat(32.0)
    };

    for (uint32_t i = 0; i < 4; ++i) {
        if (count[i] != _count[i]) {
            _count[i] = count[i];
            onBeat(i, count[i]);
        }
    }
}