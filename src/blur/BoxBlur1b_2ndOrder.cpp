
#include <assert.h>

#include "BoxBlur1b.h"

void BoxBlur1b_2ndOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius)
{
	uint32_t sumTail = 0;
	size_t i;
	for (i=0; i<1+radius*2; ++i) {
		sumTail += src[i];
	}
	uint32_t sumHead = sumTail;
	const uint8_t* pMinusTail = &src[0];
	const uint8_t* pMinusHead = pMinusTail;
	const uint8_t* pPlusHead = &src[1+radius*2];
	uint32_t sum = 0;
	for (; i<2+radius*4; ++i) {
		sum += sumHead;
		sumHead -= *pMinusHead++;
		sumHead += *pPlusHead++;
	}
	const size_t nSamples = (radius*2+1) * (radius*2+1);
	int32_t sum0 = sumHead;
	sum0 -= sumTail;
	for (i=radius*2+1; i<count-radius; ++i) {
		dst[i] = sum / nSamples;
		sum += sum0;
		sum0 += *pMinusTail++;
		sum0 -= 2 * *pMinusHead++;
		sum0 += *pPlusHead++;
	}
}

void BoxBlur1b_2ndOrder(const BoxBlur1bParams& params)
{
	const uint8_t* pSrc = params.src;
	uint8_t* pDst = params.dst;
	for (size_t i=0; i<params.height; ++i) {
		BoxBlur1b_2ndOrder(pSrc, pDst, params.width, params.hRadius >> 8);
		OffsetPtr(pSrc, params.srcLineOffsetBytes);
		OffsetPtr(pDst, params.dstLineOffsetBytes);
	}
}
