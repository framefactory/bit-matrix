/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "TestPatternEffect.h"
#include "../Fonts.h"

#include "core/math.h"

F_USE_NAMESPACE

TestPatternEffect::TestPatternEffect(int pattern, int offsetX, int offsetY, int strideX, int strideY) :
    _pattern(pattern),
    _offsetX(offsetX),
    _offsetY(offsetY),
    _strideX(strideX),
    _strideY(strideY)
{
}

void TestPatternEffect::onRender(Bitmap* pBitmap, const Timing& timing)
{
    switch(_pattern) {
        case 0:
            _drawIndex(pBitmap, timing);
            break;
        case 1:
            _drawLines(pBitmap, timing);
            break;
        case 2:
            _drawChars(pBitmap, timing);
            break;
    }
}

void TestPatternEffect::_drawIndex(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();

    int cols = w / _strideX;
    int rows = h / _strideY;

    int size = cols * rows;

    int i = 0;
    int t = int(timing.seconds * 6.0);
    int inv1 = t % size;
    int inv2 = (t + 32) % size;
    int clear1 = (t + 16) % size;
    int clear2 = (t + 48) % size;
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c, ++i) {
            char text[8];
            sprintf(text, "%02d", i);
            pBitmap->drawText(text, &Fonts::font04B24, _offsetX + c * _strideX, _offsetY + r * _strideY, 4);

            if (i == inv1 || i == inv2) {
                pBitmap->fill(_offsetX + c * _strideX, _offsetY + r * _strideY, 8, 8, Bitmap::Invert);
            }
            if (i == clear1 || i == clear2) {
                pBitmap->fill(_offsetX + c * _strideX, _offsetY + r * _strideY, 8, 8, Bitmap::Clear);
            }
        }
    }
}

void TestPatternEffect::_drawLines(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();

    int cols = w / _strideX;
    int rows = h / _strideY;

    int i = 0;
    int t = int(timing.seconds * 8.0);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c, ++i) {
            int bx = c * 8;
            int by = r * 8;
            int p = (t / 8) % 8;
            int i = t % 8;

            switch(p) {
                case 0:
                    pBitmap->line(bx, by + i, bx + 8, by + i);
                    break;
                case 1:
                    pBitmap->line(bx, by + i, bx + 8, by + i);
                    pBitmap->line(bx, by + (i + 4) % 8, bx + 8, by + (i + 4) % 8);
                    break;
                case 2:
                    pBitmap->line(bx, by + i, bx + 8, by + i);
                    pBitmap->line(bx, by + (i + 2) % 8, bx + 8, by + (i + 2) % 8);
                    pBitmap->line(bx, by + (i + 4) % 8, bx + 8, by + (i + 4) % 8);
                    break;
                case 3:
                    pBitmap->line(bx + i, by, bx + i, by + 8);
                    break;
                case 4:
                    pBitmap->line(bx + i, by, bx + i, by + 8);
                    pBitmap->line(bx + (i + 4) % 8, by, bx + (i + 4) % 8, by + 8);
                    break;
                case 5:
                    pBitmap->line(bx + i, by, bx + i, by + 8);
                    pBitmap->line(bx + (i + 2) % 8, by, bx + (i + 2) % 8, by + 8);
                    pBitmap->line(bx + (i + 4) % 8, by, bx + (i + 4) % 8, by + 8);
                    break;
                case 6:
                    pBitmap->line(bx + i, by, bx + i + 8, by + 8);
                    break;
                case 7:
                    pBitmap->line(bx + i, by + 8, bx + i + 8, by);
                    break;
            }    

            // char text[8];
            // sprintf(text, "%02d", i);
            // pBitmap->drawText(text, &Fonts::font04B24, _offsetX + c * _strideX, _offsetY + r * _strideY, 4);
        }
    }
}

void TestPatternEffect::_drawChars(Bitmap* pBitmap, const Timing& timing)
{
    int w = pBitmap->width();
    int h = pBitmap->height();

    int cols = w / _strideX;
    int rows = h / _strideY;

    int n = std::rand() % 64;
    for (int i = 0; i < n; ++i) {
        int r = std::rand() % rows;
        int c = std::rand() % cols;
        char p = std::rand() % 128;

        const char text[] { p, 0 };
        pBitmap->drawText(text, &Fonts::fontShapes1, r * 8, c * 8);
    }
}