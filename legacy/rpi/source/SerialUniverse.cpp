/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "SerialUniverse.h"
#include "SerialChain.h"

#include <wiringPi.h>
#include <algorithm>
#include <iostream>

using namespace std;


SerialUniverse::SerialUniverse(int clockPin, int loadPin, uint32_t delayMicroseconds /*= 1 */) :
    _delayMicroseconds(delayMicroseconds),
    _clockPin(clockPin),
    _loadPin(loadPin),
    _clockInverted(false),
    _loadInverted(false)
{
    pinMode(_clockPin, OUTPUT);
    pinMode(_loadPin, OUTPUT);
}

SerialUniverse::~SerialUniverse()
{
    // delete chains
    clear();
}

void SerialUniverse::setClockPinInverted(bool isInverted)
{
    _clockInverted = isInverted;
}

void SerialUniverse::setLoadPinInverted(bool isInverted)
{
    _loadInverted = isInverted;
}

void SerialUniverse::tick()
{
    digitalWrite(_clockPin, _clockInverted ? LOW : HIGH);
    delayMicroseconds(_delayMicroseconds);
    digitalWrite(_clockPin, _clockInverted ? HIGH : LOW);
    delayMicroseconds(_delayMicroseconds);
}

void SerialUniverse::load()
{
    digitalWrite(_loadPin, _loadInverted ? LOW : HIGH);
    delayMicroseconds(_delayMicroseconds);
    digitalWrite(_loadPin, _loadInverted ? HIGH : LOW);
    delayMicroseconds(_delayMicroseconds);
}

void SerialUniverse::clear()
{
    for (auto it : _chains) {
        delete it;
    }

    _chains.clear();
}

void SerialUniverse::flush()
{
    //cout << "SerialUniverse::flush\n";

    size_t count = maxBitCount();
    for (auto it : _chains) {
        it->beginWrite(count);
    }

    for (size_t i = 0; i < count; ++i) {
        for (auto it : _chains) {
            it->writeBit();
        }

        delayMicroseconds(_delayMicroseconds);
        tick();
    }

    load();
}

size_t SerialUniverse::maxBitCount() const
{
    size_t maxBitCount = 0;
    for (auto it : _chains) {
        maxBitCount = std::max(maxBitCount, it->bitCount());
    }

    //cout << "SerialUniverse::maxBitCount - " << maxBitCount << endl;
    return maxBitCount;
}
