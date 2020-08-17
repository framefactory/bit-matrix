/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

 #include "Canvas.h"
 #include "MAX7219Matrix.h"

 using namespace cimg_library;

 Canvas::Canvas(size_t width, size_t height) :
    _bitmap(width, height, 1, 1, 0)
 {
 }

void Canvas::addMatrix(MAX7219Matrix* pMatrix, uint32_t x, uint32_t y)
{
   _mappings.push_back({ pMatrix, x, y });
}

void Canvas::addMatrices(const matrixVec_t& matrices, uint32_t xPos, uint32_t yPos, uint32_t xOffset, uint32_t yOffset)
{
    for (size_t i = 0; i < matrices.size(); ++i) {
       _mappings.push_back({ matrices[i], xPos + xOffset * i, yPos + yOffset * i });
    }
}

void Canvas::drawText(const std::string& text, const bitmap_t& font, uint32_t x, uint32_t y, uint32_t stride /* = 8 */)
{
   for (size_t ic = 0; ic < text.size(); ++ic) {
      uint8_t c = text[ic];
      uint32_t cx = (c % 16) * 8;
      uint32_t cy = (c / 16) * 8;

      for (uint32_t iy = 0; iy < 8; ++iy) {
         for (uint32_t ix = 0; ix < 8; ++ix) {
            _bitmap(x + ix + ic * stride, y + iy) |= font(cx + ix, cy + iy);
         }
      }
   }
}

void Canvas::update(int xOffset /* = 0 */, int yOffset /* = 0 */)
{
   for (auto& mapping : _mappings) {
      int x = mapping.x;
      int y = mapping.y;
      for (int iy = 0; iy < 8; ++iy) {
         for (int ix = 0; ix < 8; ++ix) {
            mapping.pMatrix->setDot(ix, iy, _bitmap(x + ix + xOffset, y + iy + yOffset) != 0);            
         }
      }
   }
}