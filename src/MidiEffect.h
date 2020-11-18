/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_MIDIEFFECT_H
#define _ESP_BITMATRIX_MIDIEFFECT_H

#include "app.h"
#include "MidiTiming.h"
#include "effect/Composable.h"
#include "net/MidiMessage.h"

F_BEGIN_NAMESPACE

class MidiEffect : public Composable
{
public:
    MidiEffect(const MidiMessage& message);
    virtual ~MidiEffect() {}

    /// Starts the effect.
    void start(MidiTiming& timing);
    /// Stops the effect.
    void stop(MidiTiming& timing);

    /// Draws this layer onto the given target bitmap using the layer's blend operation.
    void render(Bitmap* pTarget, Timing& timing) override;

    /// Returns the MIDI message that triggered this effect.
    const MidiMessage& message() const { return _message; }

    /// Returns true if the effect is currently running.
    bool isActive() const { return _isActive; }

protected:
    /// Called when the effect is started.
    virtual void onStart(const MidiTiming& timing);
    /// Called when the effect is stopped.
    virtual void onStop(const MidiTiming& timing);
    /// Called when the effect should render itself. Should return true if the bitmap has changed,
    /// i.e. if any rendering has been performed.
    virtual void onRender(Bitmap* pBitmap, const MidiTiming& timing) = 0;

private:
    MidiMessage _message;
    uint64_t _offsetFrames = 0;
    double _offsetSeconds = 0.0;
    bool _isActive = false;
};
F_END_NAMESPACE

#endif // _ESP_BITMATRIX_MIDIEFFECT_H