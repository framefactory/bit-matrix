/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "AnimationEffect.h"
#include "../Fonts.h"
#include "core/math.h"

using namespace std;
F_USE_NAMESPACE

void AnimationEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    uint8_t note = startMessage().note();
    uint8_t vel = startMessage().velocity();
    
    if (note < 64) {
        animateCell(pBitmap, timing, note, vel);
    }
    else if (note < 72) {
        for (uint8_t c = 0; c < 8; ++c) {
            animateCell(pBitmap, timing, (note - 64) * 8 + c, vel);
        } 
    }
    else if (note < 80) {
        for (uint8_t r = 0; r < 8; ++r) {
            animateCell(pBitmap, timing, (note - 72) + r * 8, vel);
        } 
    }
    else if (note < 96) {
        for (uint8_t d = 0; d < 8; ++d) {
            uint8_t cell = (note - 87) + d * 9;
            if (cell >= 0 && cell < 64) {
                animateCell(pBitmap, timing, cell, vel);
            }
        }
    }
}

void AnimationEffect::animateCell(Bitmap* pBitmap, const MidiTiming& timing, uint8_t note, uint8_t vel)
{
    uint8_t x = (note % 8) * 8;
    uint8_t y = (note / 8) * 8;

    uint8_t pattern = vel / 4;
    uint8_t sub = vel % 4;

    int count, p0, p1, p2, p3;

    switch(sub) {
        case 0:
            count = timing.beatCount(64.0) % 64;
            p0 = min(count, 7);
            p1 = min(count, 23) - 16;
            p2 = min(count, 39) - 32;
            p3 = min(count, 55) - 48;
            break;
        case 1:
            count = timing.beatCount(128.0) % 64;
            p0 = min(count, 7);
            p1 = min(count, 23) - 16;
            p2 = min(count, 39) - 32;
            p3 = min(count, 55) - 48;
            break;
        case 2:
            count = timing.beatCount(32.0) % 32;
            p0 = min(count, 7);
            p1 = min(count, 15) - 8;
            p2 = min(count, 23) - 16;
            p3 = min(count, 31) - 24;
            break;
        case 3:
            count = timing.beatCount(64.0) % 32;
            p0 = min(count, 7);
            p1 = min(count, 15) - 8;
            p2 = min(count, 23) - 16;
            p3 = min(count, 31) - 24;
            break;
    }

    switch(pattern) {
        case 1:
            if (p2 < 0) pBitmap->line(x + max(0, p1), y, x + p0, y);
            if (p1 >= 0 && p3 < 0) pBitmap->line(x + 7, y + max(0, p2), x + 7, y + p1);
            if (p2 >= 0) pBitmap->line(x + 7 - p2, y + 7, x + 7 - max(0, p3), y + 7);
            if (p3 >= 0 || p1 < 0) pBitmap->line(x, y + 7 - (p3 < 0 ? 0 : p3), x, y + 7 - (p1 < 0 ? p0 : 0));
            break;
        case 2:
            pBitmap->line(x, y, x + p0, y);
            if (p1 >= 0) pBitmap->line(x + 7, y, x + 7, y + p1);
            if (p2 >= 0) pBitmap->line(x + 7 - p2, y + 7, x + 7, y + 7);
            if (p3 >= 0) pBitmap->line(x, y + 7 - p3, x, y + 7);
            break;
        case 3:
            if (p1 < 0) pBitmap->line(x + p0, y, x + p0, y + 7);
            if (p1 >= 0 && p2 < 0) pBitmap->line(x, y + p1, x + 7, y + p1);
            if (p2 >= 0 && p3 < 0) pBitmap->line(x + 7 - p2, y, x + 7 - p2, y + 7);
            if (p3 >= 0) pBitmap->line(x, y + 7 - p3, x + 7, y + 7 - p3);
            break;
        case 4:
            pBitmap->line(x + p0, y, x + p0, y + 7);
            if (p1 >= 0) pBitmap->line(x, y + p1, x + 7, y + p1);
            if (p2 >= 0) pBitmap->line(x + 7 - p2, y, x + 7 - p2, y + 7);
            if (p3 >= 0) pBitmap->line(x, y + 7 - p3, x + 7, y + 7 - p3);
            break;
        case 5:
            if (p1 < 0) pBitmap->set(x + p0, y);
            if (p1 >= 0 && p2 < 0) pBitmap->set(x + 7, y + p1);
            if (p2 >= 0 && p3 < 0) pBitmap->set(x + 7 - p2, y + 7);
            if (p3 >= 0) pBitmap->set(x, y + 7 - p3);
            break;
        case 6:
            if (p1 < 0) pBitmap->set(x + p0, y);
            if (p1 >= 0 && p2 < 0) pBitmap->set(x, y + 7 - p1);
            if (p2 >= 0 && p3 < 0) pBitmap->set(x + 7 - p2, y + 7);
            if (p3 >= 0) pBitmap->set(x + 7, y + p3);
            break;
        case 7:
            if (p1 < 0) pBitmap->set(x, y + 7 - p0);
            if (p1 >= 0 && p2 < 0) pBitmap->set(x + p1, y + p1);
            if (p2 >= 0 && p3 < 0) pBitmap->set(x + 7, y + 7 - p2);
            if (p3 >= 0) pBitmap->set(x + 7 - p3, y + p3);
            break;
        case 8:
            if (p1 < 0) pBitmap->rect(x + 7 - p0, y + 7 - p0, 2 + p0 * 2, 2 + p0 * 2);
            if (p1 >= 0 && p2 < 0) pBitmap->rect(x - 1 - p1, y - 1 - p1, 18 + p1 * 2, 18 + p1 * 2);
            if (p2 >= 0 && p3 < 0) pBitmap->rect(x - 9 - p2, y - 9 - p2, 34 + p2 * 2, 34 + p2 * 2);
            if (p3 >= 0) pBitmap->rect(x - 17 - p3, y - 17 - p3, 50 + p3 * 2, 50 + p3 * 2);
            break;
    }
}

