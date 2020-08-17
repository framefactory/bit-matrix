/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_SERIALDEVICE_H
#define _BITMATRIX_SERIALDEVICE_H

#include "app.h"


class SerialDevice
{
public:
    SerialDevice();
    virtual ~SerialDevice() {}

    bool nextBit(bool& done) const;
    virtual uint32_t bitCount() const = 0;

protected:
    void beginWrite(uint32_t data);

private:
    mutable uint32_t _data;
    mutable uint32_t _index;
};

#endif // _BITMATRIX_SERIALDEVICE_H