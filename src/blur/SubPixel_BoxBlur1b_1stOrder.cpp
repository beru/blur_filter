
#include "BoxBlur1b.h"

#include <assert.h>

void SubPixel_BoxBlur1b_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	size_t sum = 0;
	
	size_t frac = radius & 0xFF;
	size_t oneMinusFrac = 0x100 - frac;
	size_t intRad = (radius + 0xFF) >> 8;
	size_t intLen = 1 + intRad * 2;
	size_t length = 0x100 + radius * 2;
	size_t lengthInversed = 0xFFFFFF / length;
	
	for (size_t i=0; i<intLen; ++i) {
		sum += src[i];
	}
	const uint8_t* pPlus = &src[intLen];
	const uint8_t* pMinus = &src[0];

	size_t tail = src[0];
	size_t head = src[intLen-1];
	size_t hat = src[intLen];
	
	for (size_t i=intRad; i<count-intRad; ++i) {
		size_t v = (sum << 8) - (tail + head) * oneMinusFrac;
		v *= lengthInversed;
		v >>= 24;
		dst[i] = v;
		sum -= tail;
		sum += hat;
		
		++pMinus;
		tail = *pMinus;
		++pPlus;
		head = hat;
		hat = *pPlus;
	}
	
}

#include "BoxBlur1b.h"

void SubPixel_BoxBlur1b_1stOrder(const BoxBlur1bParams& params)
{
}

