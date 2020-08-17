/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_APP_H
#define _BITMATRIX_APP_H

#include <cstdint>

// MACROS
#define F_SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }

#endif // _BITMATRIX_APP_H