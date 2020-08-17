/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _BITMATRIX_CANVAS_H
#define _BITMATRIX_CANVAS_H

#include "app.h"
#include "CImg.h"

#include <vector>
#include <string>

class MAX7219Matrix;

class Canvas
{
public:
    struct mapping_t {
        MAX7219Matrix* pMatrix;
        uint32_t x;
        uint32_t y;
    };

    typedef std::vector<MAX7219Matrix*> matrixVec_t;
    typedef std::vector<mapping_t> mappingVec_t;
    typedef cimg_library::CImg<uint8_t> bitmap_t;

    Canvas(size_t width, size_t height);
    virtual ~Canvas() {};
    
    void addMatrix(MAX7219Matrix* pMatrix, uint32_t x, uint32_t y);
    void addMatrices(const matrixVec_t& matrices, uint32_t xPos, uint32_t yPos, uint32_t xOffset, uint32_t yOffset);

    void drawText(const std::string& text, const bitmap_t& font, uint32_t x, uint32_t y, uint32_t stride = 8);
    void update(int xOffset = 0, int yOffset = 0);

    size_t numMappings() { return _mappings.size(); }
    const mapping_t& mapping(size_t index) { return _mappings[index]; }

    bitmap_t* bitmap() { return &_bitmap; }

private:
    bitmap_t _bitmap;    
    mappingVec_t _mappings;
};

#endif // _BITMATRIX_CANVAS_H