/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MAX7219Chain.h"
#include "MAX7219Matrix.h"


MAX7219Chain::MAX7219Chain(uint8_t dataPin) :
    SerialChain(dataPin)
{
}

MAX7219Matrix* MAX7219Chain::createMatrix()
{
    MAX7219Matrix* pMatrix = new MAX7219Matrix();
    _devices.push_back(pMatrix);
    updateBitCount();
    return pMatrix;
}

MAX7219Chain::matrixVec_t MAX7219Chain::createMatrices(size_t count)
{
    matrixVec_t matrices; 

    for (size_t i = 0; i < count; ++i) {
        MAX7219Matrix* pMatrix = new MAX7219Matrix();
        _devices.push_back(pMatrix);
        matrices.push_back(pMatrix);
    }

    updateBitCount();
    return matrices;
}

MAX7219Matrix* MAX7219Chain::matrix(size_t index) const
{
    return static_cast<MAX7219Matrix*>(_devices[index]);
}

void MAX7219Chain::updateRow(uint8_t index)
{
    for (auto pDevice : _devices) {
        static_cast<MAX7219Matrix*>(pDevice)->updateRow(index); 
    }
}

void MAX7219Chain::updateBrightness()
{
    for (auto pDevice : _devices) {
        static_cast<MAX7219Matrix*>(pDevice)->updateBrightness(); 
    }
}

void MAX7219Chain::setBrightness(uint8_t value)
{
    for (auto pDevice : _devices) {
        static_cast<MAX7219Matrix*>(pDevice)->setBrightness(value);
    }
}

void MAX7219Chain::setRegister(uint8_t reg, uint8_t data)
{
    for (auto pDevice : _devices) {
        static_cast<MAX7219Matrix*>(pDevice)->setRegister(reg, data); 
    }
}