/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_MAX7219UNIVERSE_H
#define _BITMATRIX_MAX7219UNIVERSE_H

#include "app.h"
#include "SerialUniverse.h"

class MAX7219Chain;

class MAX7219Universe : public SerialUniverse
{
public:
    typedef std::vector<MAX7219Chain*> chainVec_t;

    MAX7219Universe(int clockPin, int loadPin, uint32_t delayMicroseconds = 1);
    virtual ~MAX7219Universe() {}

    MAX7219Chain* createChain(int dataPin);
    MAX7219Chain* chain(size_t index);
 
    void initialize();
    void shutdown();
    void update();

    void setBrightness(uint8_t value);

protected:
    void updateDisplay();
    void updateRow(uint8_t index);
    void updateBrightness();

    void setRegister(uint8_t reg, uint8_t data);
};

#endif // _BITMATRIX_MAX7219UNIVERSE_H