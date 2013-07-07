
#include <assert.h>

namespace {


} // namespace anonymous

void BoxBlur1b_3rdOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius)
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

