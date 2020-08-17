/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_MAX7219CHAIN_H
#define _BITMATRIX_MAX7219CHAIN_H

#include "app.h"
#include "SerialChain.h"

class MAX7219Matrix;


class MAX7219Chain : public SerialChain
{
public:
    typedef std::vector<MAX7219Matrix*> matrixVec_t;

    MAX7219Chain(uint8_t dataPin);
    virtual ~MAX7219Chain() {}

    MAX7219Matrix* createMatrix();
    matrixVec_t createMatrices(size_t count);
    MAX7219Matrix* matrix(size_t index) const;

    void updateRow(uint8_t index);
    void updateBrightness();
    
    void setBrightness(uint8_t value);
    void setRegister(uint8_t reg, uint8_t data);
};

#endif // _BITMATRIX_MAX7219CHAIN_H