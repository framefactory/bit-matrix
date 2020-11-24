/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_TESTPATTERNEFFECT_H
#define _ESP_BITMATRIX_TESTPATTERNEFFECT_H

#include "app.h"
#include "effect/Effect.h"

F_BEGIN_NAMESPACE

class TestPatternEffect : public Effect
{
public:
    TestPatternEffect(int pattern, int offsetX = 0, int offsetY = 0, int strideX = 8, int strideY = 8);
    void onRender(Bitmap* pBitmap, const Timing& timing) override;

private:
    void _drawIndex(Bitmap* pBitmap, const Timing& timing);
    void _drawLines(Bitmap* pBitmap, const Timing& timing);
    void _drawChars(Bitmap* pBitmap, const Timing& timing);

    int _pattern;
    int _offsetX;
    int _offsetY;
    int _strideX;
    int _strideY;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_TESTPATTERNEFFECT_H