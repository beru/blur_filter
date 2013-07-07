
#include <assert.h>

namespace {

void hblur(const uint8_t* src, uint32_t* dst, size_t count, uint8_t radius)
{
	uint32_t sum = 0;
	size_t len = 1 + radius*2;
	for (size_t i=0; i<len; ++i) {
		sum += src[i];
	}
	const uint8_t* pPlus = &src[len];
	const uint8_t* pMinus = &src[0];
	
	for (size_t i=radius; i<count-radius; ++i) {
		dst[i] = sum;
		sum += *pPlus++;
		sum -= *pMinus++;
	}
}

void vblur()


} // namespace anonymous

void BoxBlur1b_1stOrder(const uint8_t* src, uint8_t* dst, size_t count, uint8_t radius)
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

