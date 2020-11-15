/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_APP_H
#define _ESP_BITMATRIX_APP_H

#include <Arduino.h>
#include <cstdint>

// DEBUG
#define F_DEBUG 1

// MACROS
#define F_BEGIN_NAMESPACE namespace ff {
#define F_END_NAMESPACE }
#define F_USE_NAMESPACE using namespace ff;

#define F_SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }


#endif // _ESP_BITMATRIX_APP_H