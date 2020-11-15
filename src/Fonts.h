/**
 * ESP/Arduino Font Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_FONTS_H
#define _ESP_BITMATRIX_FONTS_H

#include "app.h"
#include "effect/Bitmap.h"

F_BEGIN_NAMESPACE

struct Fonts
{
    static Bitmap font04B24;
    static Bitmap fontC648;
    static Bitmap fontC64;
    static Bitmap fontShapes1;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_FONTS_H