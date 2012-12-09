
#include "box_blur_1b.h"

#include <assert.h>

void BoxBlur_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	uint32_t sum = 0;
	for (size_t i=0; i<1+radius*2; ++i) {
		sum += src[i];
	}
	const uint8_t* pPlus = &src[1+radius*2];
	const uint8_t* pMinus = &src[0];
	
	for (size_t i=radius; i<count-radius; ++i) {
		uint32_t v = sum / (1+radius*2);
		assert(v < 256);
		dst[i] = v;
		sum += *pPlus++;
		sum -= *pMinus++;
	}
}

void BoxBlur_2ndOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	uint32_t sumTail = 0;
	size_t i;
	for (i=0; i<1+radius*2; ++i) {
		sumTail += src[i];
	}
	uint32_t sumHead = sumTail;
	const uint8_t* pPlusTail = &src[1+radius*2];
	const uint8_t* pMinusTail = &src[0];
	const uint8_t* pPlusHead = pPlusTail;
	const uint8_t* pMinusHead = pMinusTail;
	uint32_t sum = 0;
	for (; i<2+radius*4; ++i) {
		sum += sumHead;
		sumHead += *pPlusHead++;
		sumHead -= *pMinusHead++;
	}
	const size_t nSamples = (radius*2+1) * (radius*2+1);
	for (; i<count-radius; ++i) {
		dst[i] = sum / nSamples;
		sum += sumHead;
		sum -= sumTail;
		sumTail -= *pMinusTail++;
		sumTail += *pPlusTail++;
		sumHead -= *pMinusHead++;
		sumHead += *pPlusHead++;
	}
}

void BoxBlur_3rdOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	
}

