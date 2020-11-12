/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_SPIRAL_H
#define _ESP_BITMATRIX_SPIRAL_H

#include "app.h"
#include "effect/Effect.h"

F_BEGIN_NAMESPACE

class Spiral : public Effect
{
public:
    void onRender(Bitmap* pBitmap, const Timing& timing);

private:
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_SPIRAL_H