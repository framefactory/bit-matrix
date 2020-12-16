/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_DOTMATRIX_MIDITIMING_H
#define _ESP_DOTMATRIX_MIDITIMING_H

#include "app.h"
#include "effect/Timing.h"

F_BEGIN_NAMESPACE

struct MidiTiming : public Timing
{
    struct text_t {
        text_t() : x(0), y(0) {}
        uint32_t x;
        uint32_t y;
        std::string text;
    };

    MidiTiming();

    /// Pitch bend value
    float pitchBend;
    /// Controller values
    float control[10];
    /// Text stores
    text_t text[16];
};

F_END_NAMESPACE

#endif // _ESP_DOTMATRIX_MIDITIMING_H