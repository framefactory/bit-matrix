/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_LINES_H
#define _ESP_BITMATRIX_LINES_H

#include "app.h"
#include "effect/Effect.h"

F_BEGIN_NAMESPACE

class Lines : public Effect
{
public:
    void onRender(Bitmap* pBitmap, const Timing& timing) override;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_LINES_H