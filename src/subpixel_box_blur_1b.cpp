#include "subpixel_box_blur_1b.h"

#include <assert.h>

void SubPixel_BoxBlur_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	size_t sum = 0;
	
	size_t oneMinusFrac = 0xFF - (radius & 0xFF);
	size_t intRad = (radius + 0xFF) >> 8;
	size_t intLen = 1 + intRad * 2;
	size_t len = 0x100 + radius * 2;
	size_t invLen = 0xFFFFFF / len;
	
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
		v *= invLen;
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

