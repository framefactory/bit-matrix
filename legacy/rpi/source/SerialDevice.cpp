/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "SerialDevice.h"
#include <iostream>

using namespace std;


SerialDevice::SerialDevice()
{
}

bool SerialDevice::nextBit(bool& done) const
{
    _index--;
    done = !_index;

    return (_data & (1 << _index)) != 0;
} 

void SerialDevice::beginWrite(uint32_t data)
{
    _data = data;
    _index = bitCount();

    //cout << "SerialDevice::beginWrite - " << _index << "bits, value: " << _data << endl;
}