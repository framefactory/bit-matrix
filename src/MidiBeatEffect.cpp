/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiBeatEffect.h"

F_USE_NAMESPACE

void MidiBeatEffect::dispatchBeats(const MidiTiming& timing)
{
    uint32_t count[4] {
        timing.beatCount(4.0),
        timing.beatCount(8.0),
        timing.beatCount(16.0),
        timing.beatCount(32.0)
    };

    for (uint32_t i = 0; i < 4; ++i) {
        if (count[i] != _count[i]) {
            _count[i] = count[i];
            onBeat(i, count[i]);
        }
    }
}