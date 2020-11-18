/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_PARTICLEEFFECT_H
#define _ESP_BITMATRIX_PARTICLEEFFECT_H

#include "../app.h"
#include "../MidiEffect.h"

#include "core/Particle1T.h"

F_BEGIN_NAMESPACE

class ParticleEffect : public MidiEffect
{
public:
    ParticleEffect(const MidiMessage& message) :
        MidiEffect(message) {}
        
protected:
    void onStart(const MidiTiming& timing) override;
    void onRender(Bitmap* pBitmap, const MidiTiming& timing) override;
    Bitmap::DrawOp calcBlinkOp(int index, float delta, float lower, float upper);

private:
    Particle1f _particles[64];
    float _rate[64];
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_PARTICLEEFFECT_H