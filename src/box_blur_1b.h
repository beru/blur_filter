#pragma once

typedef void (*BoxBlurFunc)(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius);

void BoxBlur_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius);
void BoxBlur_2ndOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius);
void BoxBlur_3rdOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius);
