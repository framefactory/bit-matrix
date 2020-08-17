/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_MAX7219MATRIX_H
#define _BITMATRIX_MAX7219MATRIX_H

#include "app.h"
#include "SerialDevice.h"
#include <string>

class SerialChain;

class MAX7219Matrix : public SerialDevice
{
public:
    enum Register {
        NO_OP       = 0x00,
        DIGIT_0     = 0x01,
        DIGIT_1     = 0x02,
        DIGIT_2     = 0x03,
        DIGIT_3     = 0x04,
        DIGIT_4     = 0x05,
        DIGIT_5     = 0x06,
        DIGIT_6     = 0x07,
        DIGIT_7     = 0x08,
        DECODE_MODE = 0x09,
        INTENSITY   = 0x0a,
        SCAN_LIMIT  = 0x0b,
        SHUTDOWN    = 0x0c,
        TEST        = 0x0f,
    };

    MAX7219Matrix();
    virtual ~MAX7219Matrix() {};

    void setDot(uint8_t x, uint8_t y, bool state) {
        _row[y] = state ? _row[y] | (1 << x) : _row[y] & ~(1 << x);
    }

    void setRow(uint8_t index, uint8_t  value);
    void setRow(uint8_t index, const std::string& sequence);

    void clear();
    void clearRow(uint8_t index);

    uint8_t row(uint8_t index) { return _row[index]; }
    bool rowChanged(uint8_t index) { return _rowChanged[index]; }

    void setBrightness(uint8_t brightness);
    uint8_t brightness() const { return _brightness; }
    bool brightnessChanged() { return _brightnessChanged; }

    void updateRow(uint8_t index);
    void updateBrightness();
    
    void setRegister(uint8_t reg, uint8_t data);

    virtual uint32_t bitCount() const override { return 16; }

private:
    uint8_t _row[8];
    uint8_t _brightness;
    bool _rowChanged[8];
    bool _brightnessChanged;
};

#endif // _MAX7219MATRIX_H