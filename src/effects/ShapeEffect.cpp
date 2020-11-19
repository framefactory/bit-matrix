/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ShapeEffect.h"
#include "../Fonts.h"
#include "core/math.h"
#include <cmath>

F_USE_NAMESPACE

void ShapeEffect::onStart(const MidiTiming& timing, const MidiMessage& message)
{
    _generator.setSeed(std::rand());
}

void ShapeEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    uint8_t note = startMessage().note();
    uint8_t vel = startMessage().velocity();

    char text[2] = { vel, 0 };

    uint32_t i = 0, x = 0, y = 0, w = 0, h = 0;

    if (note < 64) {
        x = note % 8;
        y = note / 8;
    }
    else if (note < 96) {
        dispatchBeats(timing);
        int rnd = _random[(note - 64) / 8];
        int rnd64 = rnd % 64;

        switch(note % 16) {
            case 0: // random whole area
                x = rnd64 / 8; y = rnd64 % 8;
                break;
            case 1: // inner 4 x 4
                x = 2 + rnd64 % 4; y = 2 + rnd64 / 16;
                break;
            case 2: // second outer ring
                i = rnd64 % 20;
                x = i < 6 ? i : (i < 11 ? 6 : (i < 16 ? 16 - i : 1));
                y = i < 6 ? 1 : (i < 11 ? i - 4 : ( i < 16 ? 6 : 21 - i));
                break;
            case 3: // outer most ring
                i = rnd64 % 28;
                x = i < 8 ? i : (i < 15 ? 7 : (i < 22 ? 21 - i : 0));
                y = i < 8 ? 0 : (i < 15 ? i - 7 : ( i < 22 ? 7 : 28 - i));
                break;
            case 4: // random block 2 x 2
                x = (rnd64 / 7) % 7; y = rnd64 % 7;
                w = 2; h = 2;
                break;
            case 5: // random block 4 x 1
                x = rnd64 % 5; y = rnd64 / 8;
                w = 4; h = 1;
                break;
            case 6: // random block 1 x 4
                x = rnd64 % 8; y = (rnd64 / 8) % 5;
                w = 1; h = 4;
                break;
            case 7: // random row
                x = 0; y = rnd64 % 8;
                w = 8; h = 1;
                break;
        }
    }    
    if (w == 0) {
        pBitmap->drawText(text, &Fonts::fontShapes1, x * 8, y * 8);
    }
    else {
        for (uint32_t hi = 0; hi < h; ++hi) {
            for (uint wi = 0 ; wi < w; ++wi) {
                pBitmap->drawText(text, &Fonts::fontShapes1, (x + wi) * 8, (y + hi) * 8);
            }
        }
    }
}

void ShapeEffect::onBeat(uint32_t index, uint32_t count)
{
    _random[index] = _generator.generateInt();
}
