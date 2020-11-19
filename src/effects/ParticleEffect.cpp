/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ParticleEffect.h"
#include "core/math.h"

F_USE_NAMESPACE

void ParticleEffect::onStart(const MidiTiming& timing, const MidiMessage& message)
{
}

void ParticleEffect::onStop(const MidiTiming& timing, const MidiMessage& message)
{
    uint8_t mode = (startMessage().note() / 2) % 4;
    uint8_t vel = stopMessage().velocity();
    float f = vel == 0 ? 0.7f : vel / 127.0f;

    for (int i = 0; i < 64; ++i) {
        _rate[i] = Math::random(0.5f, 2.0f);
        float s = mode == 0 ? f : (mode == 1 ? -f : (rand() % 2 ? f : -f));

        _particles[i].position = 0;
        _particles[i].velocity = 5.0f + s * Math::random(25.0f, 80.0f);
        _particles[i].damping = Math::random(0.9, 0.99);
    }

    _releaseEnd = timing.effectSeconds + 5.0;
}

void ParticleEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    bool inRelease = !isActive();

    uint8_t note = startMessage().note();
    uint8_t note6 = note % 8;
    uint8_t vel64 = Math::limit(startMessage().velocity() / 2 + int(timing.pitchBend * 64), 0, 63);

    if (note6 == 0 || note6 == 1 || note6 == 4) {
        vel64 = 63 - vel64;
    }

    int bx = vel64 % 8;
    int by = vel64 / 8;

    if (note < 8) {
        for (int i = 0; i < 64; ++i) {
            auto& p = _particles[i];
            auto& p1 = _particles[(i * 7 + 17) % 64];

            if (note < 6) {
                    if (note % 2) {
                        pBitmap->set(vel64 + p.position, i, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                    }
                    else {
                        pBitmap->set(i, vel64 + p.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                    }
            }
            else {
                if (note % 2) {
                    pBitmap->set(vel64 + p.position, i + p1.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
                else {
                    pBitmap->set(i + p.position, vel64 + p1.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
            }
        }
    }
    else if (note < 16) {
        for (int i = 0; i < 64; ++i) {
            int px = i % 8;
            int py = i / 8;

            auto& p = _particles[i];
            auto& p1 = _particles[(i * 3 + 17) % 64];

            if (note < 12) {
                if (note % 2) {
                    pBitmap->set(bx * 8 + px + p.position, by * 8 + py, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
                else {
                    pBitmap->set(bx * 8 + px, by * 8 + py + p.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
            }
            else {
                pBitmap->set(bx * 8 + px + p.position, by * 8 + py + p1.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
            }
        }
    }
    else if (note < 24) {
        for (int i = 0; i < 64; ++i) {
            int px = (i % 2) + 3;
            int py = i / 2;

            auto& p = _particles[i];
            auto& p1 = _particles[(i * 3 + 17) % 64];

            if (note < 22) {
                if (note % 2) {
                    pBitmap->set(bx * 8 + px + p.position, by * 8 + py, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
                else {
                    pBitmap->set(bx * 8 + px, by * 8 + py + p.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
                }
            }
            else {
                pBitmap->set(bx * 8 + px + p.position, by * 8 + py + p1.position, calcBlinkOp(i, timing.delta, 0.1f, 1.0f));
            }
        }
    }

    if (inRelease) {
        if (timing.effectSeconds < _releaseEnd) {
            for (int i = 0; i < 64; ++i) {
                _particles[i].update(timing.delta);
            }
        }
        else {
            setEnabled(false);
        }
    }
}

Bitmap::DrawOp ParticleEffect::calcBlinkOp(int index, float delta, float lower, float upper)
{
    if (isActive()) {
        return Bitmap::Set;
    }

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