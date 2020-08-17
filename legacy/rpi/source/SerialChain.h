/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_SERIALCHAIN_H
#define _BITMATRIX_SERIALCHAIN_H

#include "app.h"
#include <vector>

class SerialDevice;


class SerialChain
{
public:
    typedef std::vector<SerialDevice*> deviceVec_t;

    SerialChain(uint8_t dataPin);
    virtual ~SerialChain();

    void setDataPinInverted(bool isInverted);

    void beginWrite(uint32_t totalBitCount);
    void writeBit();

    void clear();
    uint32_t deviceCount() const { return _devices.size(); }
    uint32_t bitCount() const { return _bitCount; }

protected:
    void updateBitCount();
    deviceVec_t _devices;

private:
    uint32_t _bitCount;
    uint8_t _dataPin;
    uint8_t _dataInverted;

    mutable uint32_t _deviceIndex;
    mutable uint32_t _bitOffset;
};

#endif // _BITMATRIX_SERIALCHAIN_H