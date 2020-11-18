/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ParticleEffect.h"
#include "core/math.h"

F_USE_NAMESPACE

void ParticleEffect::onStart(const MidiTiming& timing)
{
    uint8_t note = message().note();
    uint8_t vel = message().velocity();

    for (int i = 0; i < 64; ++i) {
        _rate[i] = Math::random(0.5f, 2.0f);
    }

    switch(note) {
    case 0:
        for (int i = 0; i < 64; ++i) {
            _particles[i].position = 0;
            _particles[i].velocity = vel * 0.1f;
            _particles[i].acceleration = 10.0f + Math::random(-5.0f, 10.0f);
        }
        break;

    case 1:
        for (int i = 0; i < 64; ++i) {
            float f = Math::random(7.0f, 70.0f);
            f = rand() % 2 ? f : -f;
            _particles[i].position = 0;
            _particles[i].velocity = f;
            _particles[i].damping = Math::random(0.8, 0.95);
        }
        break;

    }

}

void ParticleEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    uint8_t note = message().note();
    uint8_t vel = message().velocity();

    int bx = (vel / 2) % 8;
    int by = vel / 16;

    for (int i = 0; i < 64; ++i) {
        auto& p = _particles[i];

        switch(note) {
            case 0: {
                pBitmap->set(i, p.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                break;
            }
            case 1: {
                int x = i % 8;
                int y = i / 8;
                pBitmap->set(bx * 8 + x + p.position, by * 8 + y, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                break;
            }
        }

        p.update(timing.delta);
    }
}

Bitmap::DrawOp ParticleEffect::calcBlinkOp(int index, float delta, float lower, float upper)
{
    if (_rate[index] > 0.0f) {
        _rate[index] -= delta;
        if (_rate[index] < 0.0f) {
            _rate[index] = Math::random(-upper, -lower);
        }

        return Bitmap::Set;
    }

    _rate[index] += delta;
    if (_rate[index] > 0.0f) {
        _rate[index] = Math::random(lower, upper);
    }

    return Bitmap::Clear;
}