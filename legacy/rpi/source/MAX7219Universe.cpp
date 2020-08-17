/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MAX7219Universe.h"
#include "MAX7219Chain.h"
#include "MAX7219Matrix.h"

#include <iostream>

using namespace std;


MAX7219Universe::MAX7219Universe(int clockPin, int loadPin, uint32_t delayMicroseconds /* = 1 */) :
    SerialUniverse(clockPin, loadPin, delayMicroseconds)
{
}

MAX7219Chain* MAX7219Universe::createChain(int dataPin)
{
    MAX7219Chain* pChain = new MAX7219Chain(dataPin);
    _chains.push_back(pChain);
    return pChain;
}

MAX7219Chain* MAX7219Universe::chain(size_t index)
{
    return static_cast<MAX7219Chain*>(_chains[index]);
}

void MAX7219Universe::initialize()
{
    setRegister(MAX7219Matrix::Register::TEST, 0x00);
    setRegister(MAX7219Matrix::Register::SHUTDOWN, 0x01);
    setRegister(MAX7219Matrix::Register::DECODE_MODE, 0x00);
    setRegister(MAX7219Matrix::Register::SCAN_LIMIT, 0x07);
    updateBrightness();
}

void MAX7219Universe::shutdown()
{
    setRegister(MAX7219Matrix::Register::SHUTDOWN, 0x00);
}

void MAX7219Universe::update()
{
    updateBrightness();
    updateDisplay();
}

void MAX7219Universe::setBrightness(uint8_t value)
{
    for (auto pChain : _chains) {
        static_cast<MAX7219Chain*>(pChain)->setBrightness(value);
    }
}

void MAX7219Universe::updateDisplay()
{
    for (uint8_t i = 0; i < 8; ++i) {
        updateRow(i);
    }
} 

void MAX7219Universe::updateRow(uint8_t index)
{
    for (auto pChain : _chains) {
        static_cast<MAX7219Chain*>(pChain)->updateRow(index);
    }

    flush();
}

void MAX7219Universe::updateBrightness()
{
    for (auto pChain : _chains) {
        static_cast<MAX7219Chain*>(pChain)->updateBrightness();
    }

    flush();
}

void MAX7219Universe::setRegister(uint8_t reg, uint8_t data)
{
    //cout << "MAX7219Universe::writeRegister - " << (int)reg << ": " << (int)data << endl;

    for (auto pChain : _chains) {
        static_cast<MAX7219Chain*>(pChain)->setRegister(reg, data);
    }

    flush();
}
