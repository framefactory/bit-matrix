/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiBeatEffect.h"

F_USE_NAMESPACE

void MidiBeatEffect::dispatchBeats(const Timing& timing)
{
    uint32_t b32count = timing.beat(32.0);
    if (b32count == _b32count) {
        return;
    }

    _b32count = b32count;
    onB32(b32count);

    uint32_t b16count = timing.beat(16.0);
    if (b16count == _b16count) {
        return;
    }

    _b16count = b16count;
    onB16(b16count);

    uint32_t b8count = timing.beat(8.0);
    if (b8count == _b8count) {
        return;
    }

    _b8count = b8count;
    onB8(b8count);

    uint32_t b4count = timing.beat(4.0);
    if (b4count == _b4count) {
        return;
    }

    _b4count = b4count;
    onB4(b4count);

    uint32_t b2count = timing.beat(2.0);
    if (b2count == _b2count) {
        return;
    }

    _b2count = b2count;
    onB2(b2count);

    uint32_t b1count = timing.beat(1.0);
    if (b1count == _b1count) {
        return;
    }

    _b1count = b1count;
    onB1(b1count);
}