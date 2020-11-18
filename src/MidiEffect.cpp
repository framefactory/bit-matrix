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

void MidiEffect::start(MidiTiming& timing)
{
    _isActive = true;
    _offsetFrames = timing.frames;
    _offsetSeconds = timing.seconds;
    onStart(timing);
}

void MidiEffect::stop(MidiTiming& timing)
{
    onStop(timing);
    _isActive = false;
}

void MidiEffect::render(Bitmap* pTarget, Timing& timing)
{
    if (!enabled()) {
        return;
    }

    MidiTiming& midiTiming = static_cast<MidiTiming&>(timing);
    
    midiTiming.effectFrames = midiTiming.frames - _offsetFrames;
    midiTiming.effectSeconds = midiTiming.seconds - _offsetSeconds;

    onRender(pTarget, midiTiming);
}

void MidiEffect::onStart(const MidiTiming& timing)
{
}

void MidiEffect::onStop(const MidiTiming& timing)
{
}
