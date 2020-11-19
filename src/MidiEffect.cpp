/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiEffect.h"

F_USE_NAMESPACE

void MidiEffect::start(MidiTiming& timing, const MidiMessage& message)
{
    _startMessage = message;

    _offsetFrames = timing.frames;
    _offsetSeconds = timing.seconds;

    _isActive = true;
    onStart(timing, _startMessage);
}

void MidiEffect::stop(MidiTiming& timing, const MidiMessage& message)
{
    _stopMessage = message;

    // covert NoteOn with velocity 0 to NoteOff
    if (_stopMessage.status() == MidiStatus::NoteOn && _stopMessage.velocity() == 0) {
        _stopMessage.setBytes(
            MidiStatus::NoteOff + _stopMessage.channel(),
            _stopMessage.note(),
            _stopMessage.velocity()
        );
    }

    _isActive = false;
    onStop(timing, _stopMessage);
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

void MidiEffect::onStart(const MidiTiming& timing, const MidiMessage& message)
{
}

void MidiEffect::onStop(const MidiTiming& timing, const MidiMessage& message)
{
}
