/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "SerialChain.h"
#include "SerialDevice.h"

#include <wiringPi.h>
#include <iostream>

using namespace std;


SerialChain::SerialChain(uint8_t dataPin) :
    _bitCount(0),
    _dataPin(dataPin),
    _dataInverted(false)
{
    pinMode(_dataPin, OUTPUT);
}

SerialChain::~SerialChain()
{
    clear();
}

void SerialChain::clear()
{
    for(auto it : _devices) {
        delete it;
    }

    _bitCount = 0;
}

void SerialChain::setDataPinInverted(bool isInverted)
{
    _dataInverted = isInverted;
}

void SerialChain::beginWrite(uint32_t totalBitCount)
{
    _deviceIndex = deviceCount() - 1;
    _bitOffset = totalBitCount - _bitCount;

    //cout << "SerialChain::beginWrite - offset: " << _bitOffset << ", ";    
}

void SerialChain::writeBit()
{
    if (_bitOffset > 0) {
        _bitOffset--;
        digitalWrite(_dataPin, _dataInverted ? HIGH : LOW);
        return;
    }

    bool done = false;
    bool bit = _devices[_deviceIndex]->nextBit(done);
    if (done) {
        _deviceIndex--;
    }

    //cout << (bit ? "1" : "0");
    //if (done) cout << endl;
    digitalWrite(_dataPin, _dataInverted ^ bit ? HIGH : LOW);
}

void SerialChain::updateBitCount()
{
    uint32_t bitCount = 0;
    for (auto it : _devices) {
        bitCount += it->bitCount();
    }

    _bitCount = bitCount;
}


