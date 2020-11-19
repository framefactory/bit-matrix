/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "StaticKeyEffect.h"
#include "core/math.h"

#include <cstdlib>

F_USE_NAMESPACE


void StaticKeyEffect::onRender(Bitmap* pBitmap, const MidiTiming& timing)
{
    if (!isActive()) {
        setEnabled(false);
        return;
    }

    int note = startMessage().note();
    int vel = startMessage().velocity();
    int v64 = Math::limit(vel - 32, 0, 127);
    int v32 = v64 >> 1;

    int sx = pBitmap->width();
    int cx = sx >> 1;
    int sy = pBitmap->height();
    int cy = sy >> 1;

    int b8 = timing.beat(8.0);
    if (b8 != _b8) {
        _b8 = b8;
        _r8 = random();
    }
    int b16 = timing.beat(16.0);
    if (b16 != _b16) {
        _b16 = b16;
        _r16 = random();
    }
    int b32 = timing.beat(32.0);
    if (b32 != _b32) {
        _b32 = b32;
        _r32 = random();
    }

    switch(note) {
    case 0:
        pBitmap->line(0, v64, sx, v64);
        break;
    case 1:
        pBitmap->line(0, v64 + b8, sx, v64 + b8);
        break;
    case 2:
        pBitmap->line(0, v64 + b16, sx, v64 + b16);
        break;
    case 3:
        pBitmap->line(0, v64 + b32, sx, v64 + b32);
        break;
    case 4:
        pBitmap->line(0, v64 - b8, sx, v64 - b8);
        break;
    case 5:
        pBitmap->line(0, v64 - b16, sx, v64 - b16);
        break;
    case 6:
        pBitmap->line(0, v64 - b32, sx, v64 - b32);
        break;
    case 7:
        pBitmap->line(0, v64 - b32, sx, v64 + b32);
        break;
    case 8:
        pBitmap->line(0, v64 + b32, sx, v64 - b32);
        break;

    case 12:
        pBitmap->line(v64, 0, v64, sy);
        break;
    case 13:
        pBitmap->line(v64 + b8, 0, v64 + b8, sy);
        break;
    case 14:
        pBitmap->line(v64 + b16, 0, v64 + b16, sy);
        break;
    case 15:
        pBitmap->line(v64 + b16, 0, v64 + b32, sy);
        break;
    case 16:
        pBitmap->line(v64 - b8, 0, v64 - b8, sy);
        break;
    case 17:
        pBitmap->line(v64 - b16, 0, v64 - b16, sy);
        break;
    case 18:
        pBitmap->line(v64 - b32, 0, v64 - b32, sy);
        break;
    case 19:
        pBitmap->line(v64 - b32, 0, v64 + b32, sy);
        break;
    case 20:
        pBitmap->line(v64 + b32, 0, v64 - b32, sy);
        break;

    case 24:
        pBitmap->rect(cx - v32, cy - v32, v64, v64);
        break;
    case 25:
        pBitmap->circle(cx, cy, v32);
        break;

    case 36: {
        int row = int(_r8 * 64.0) % 8;
        int col = int(_r8 * 64.0) >> 3;
        pBitmap->rect(row * 8, col * 8, 8, 8);
    } break;
    case 37: {
        int row = int(_r16 * 64.0) % 8;
        int col = int(_r16 * 64.0) >> 3;
        pBitmap->rect(row * 8, col * 8, 8, 8);
    } break;
    case 38: {
        int row = int(_r32 * 64.0) % 8;
        int col = int(_r32 * 64.0) >> 3;
        pBitmap->rect(row * 8, col * 8, 8, 8);
    } break;

    case 39: {
        int row = int(_r8 * 64.0) % 8;
        int col = int(_r8 * 64.0) >> 3;
        pBitmap->rect(row * 8 + 2, col * 8 + 2, 4, 4);
    } break;
    case 40: {
        int row = int(_r16 * 64.0) % 8;
        int col = int(_r16 * 64.0) >> 3;
        pBitmap->rect(row * 8 + 2, col * 8 + 2, 4, 4);
    } break;
    case 41: {
        int row = int(_r32 * 64.0) % 8;
        int col = int(_r32 * 64.0) >> 3;
        pBitmap->rect(row * 8 + 2, col * 8 + 2, 4, 4);
    } break;

    case 42: {
        int row = int(_r8 * 64.0) % 8;
        int col = int(_r8 * 64.0) >> 3;
        pBitmap->fill(row * 8 + 3, col * 8 + 3, 2, 2);
    } break;
    case 43: {
        int row = int(_r16 * 64.0) % 8;
        int col = int(_r16 * 64.0) >> 3;
        pBitmap->fill(row * 8 + 3, col * 8 + 3, 2, 2);
    } break;
    case 44: {
        int row = int(_r32 * 64.0) % 8;
        int col = int(_r32 * 64.0) >> 3;
        pBitmap->fill(row * 8 + 3, col * 8 + 3, 2, 2);
    } break;

    }
}

double StaticKeyEffect::random()
{
    return double(std::rand()) / double(RAND_MAX);
}