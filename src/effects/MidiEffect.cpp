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
