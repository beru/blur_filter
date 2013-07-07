#pragma once

void BoxBlur1b_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius);
void BoxBlur1b_2ndOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius);
void BoxBlur1b_3rdOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius);

