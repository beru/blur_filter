
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

void BoxBlur_1stOrder_SubPixel(const uint8_t* src, uint8_t* dst, size_t count, float radius)
{
}

void BoxBlur_2ndOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
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

void BoxBlur_3rdOrder(const uint8_t* src, uint8_t* dst, size_t count, uint16_t radius)
{
	int32_t tmp;
	int32_t sum1 = 0;
	int32_t sum2 = 0;
	int32_t sum3 = 0;
	size_t i;
	size_t len = 1 + radius*2;
	// box sum
	for (i=0; i<len; ++i) {
		sum1 += src[i];
	}
	// tent sum
	int32_t initial_sum1 = sum1;
	for (i=0; i<len; ++i) {
		sum2 += sum1;
		sum1 -= src[i];
		sum1 += src[i+len];
	}
	// quadratic sum
	tmp = -initial_sum1 + sum1; // calc tent filter diff
	int32_t initial_sum2 = sum2;
	for (i=0; i<len; ++i) {
		sum3 += sum2;
		sum2 += tmp;
		tmp += src[i];
		tmp -= src[i+len] * 2;
		tmp += src[i+len*2];
	}
	sum2 -= initial_sum2;
	tmp = sum1;
	sum1 += initial_sum1;
	for (i=len; i<len*2; ++i) {
		sum1 += src[len+i];
	}
	for (i=len; i<count-len; ++i) {
		assert(sum3 >= 0);
		dst[i+radius] = sum3 / (len*len*len);
//		printf("%d %d %d\n", src[i], dst[i], sum2);
		sum3 += sum2;
		sum2 += sum1;
		sum2 -= tmp * 3;
		sum1 -= src[i-len];
		sum1 += src[i+len*2];
		tmp  -= src[i];
		tmp  += src[i+len];
	}
}
