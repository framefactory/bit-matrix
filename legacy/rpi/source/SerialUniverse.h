/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_SERIALUNIVERSE_H
#define _BITMATRIX_SERIALUNIVERSE_H

#include "app.h"
#include <vector>

class SerialChain;


class SerialUniverse
{
public:
    typedef std::vector<SerialChain*> chainVec_t;

    SerialUniverse(int clockPin, int loadPin, uint32_t delayMicroseconds = 1);
    virtual ~SerialUniverse();

    void setClockPinInverted(bool isInverted);
    void setLoadPinInverted(bool isInverted);

    void tick();
    void load();

    void clear();

    const SerialChain* chain(size_t index) { return _chains[index]; }
    const chainVec_t& chains() const { return _chains; }

    void flush();
    size_t maxBitCount() const;

protected:
    chainVec_t _chains;

private:
    uint32_t _delayMicroseconds;
    int _clockPin;
    int _loadPin;
    bool _clockInverted;
    bool _loadInverted;
};

#endif // _BITMATRIX_SERIALUNIVERSE_H