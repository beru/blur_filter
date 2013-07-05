#pragma once

// value of radius parameter is fixed point value.
// decimal part : 8bit
// fractional part : 8bit

void SubPixel_BoxBlur_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius);

