/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MAX7219Matrix.h"

MAX7219Matrix::MAX7219Matrix() :
    _row { 0, 0, 0, 0, 0, 0, 0, 0 },
    _brightness(0x04),
    _rowChanged { true, true, true, true, true, true, true, true },
    _brightnessChanged(true)
{
}

void MAX7219Matrix::setRow(uint8_t index, uint8_t value)
{
    if (_row[index] != value) {
        _row[index] = value;
        _rowChanged[index] = true;
    }
}

void MAX7219Matrix::setRow(uint8_t index, const std::string& sequence)
{
    _row[index] = 0;
    for (uint8_t c = 0; c < 8; ++c) {
        if(sequence[c] != '0' && sequence[c] != ' ') {
            _row[index] += 1 << c;
        }
    }
}

void MAX7219Matrix::clear()
{
    for (uint8_t i = 0; i < 8; ++i) {
        clearRow(i);
    }
}

void MAX7219Matrix::clearRow(uint8_t index)
{
    setRow(index, 0);
}

void MAX7219Matrix::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    _brightnessChanged = true;
}

void MAX7219Matrix::updateRow(uint8_t index)
{
    beginWrite(((8 - index) << 8) + _row[index]);
    _rowChanged[index] = false;
}

void MAX7219Matrix::updateBrightness()
{
    beginWrite((Register::INTENSITY << 8) + _brightness);
    _brightnessChanged = false;
}

void MAX7219Matrix::setRegister(uint8_t reg, uint8_t data)
{
    beginWrite((reg << 8) + data);
}