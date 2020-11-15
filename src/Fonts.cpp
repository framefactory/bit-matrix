/**
 * ESP/Arduino Font Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Fonts.h"

F_USE_NAMESPACE

extern const uint8_t data_dm8_04b24[] asm("_binary_bin_dm8_04b24_bin_start");
extern const uint8_t data_dm8_c64[] asm("_binary_bin_dm8_c64_bin_start");
extern const uint8_t data_dm8_c648[] asm("_binary_bin_dm8_c648_bin_start");
extern const uint8_t data_dm8_shapes1[] asm("_binary_bin_dm8_shapes1_bin_start");

Bitmap Fonts::font04B24(128, 64, data_dm8_04b24, false);
Bitmap Fonts::fontC64(128, 64, data_dm8_c64, false);
Bitmap Fonts::fontC648(128, 64, data_dm8_c648, false);
Bitmap Fonts::fontShapes1(128, 64, data_dm8_shapes1, false);