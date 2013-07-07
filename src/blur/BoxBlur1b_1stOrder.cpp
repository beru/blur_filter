
#include <assert.h>

#include "BoxBlur1b.h"
#include "RingLinePtr.h"
#include <vector>

void BoxBlur1b_1stOrder(const BoxBlur1bParams& params)
{
	uint32_t hRad = params.hRadius >> 8;
	uint32_t vRad = params.vRadius >> 8;
	uint32_t hLen = 1 + hRad*2;
	uint32_t vLen = 1 + vRad*2;
	uint32_t invLen = 0xFFFFFF / (hLen*vLen);
	uint32_t hCount = params.width;
	uint32_t vCount = params.height;

	const uint8_t* hLine = params.src;
	uint8_t* vLine = params.dst;

	OffsetPtr(vLine, params.dstLineOffsetBytes * vRad);

	std::vector<uint32_t> vSumLine(params.width);
	std::vector<uint16_t> work(params.width * vLen);
	uint16_t* pWork = &work[0];
	
	RingLinePtr<uint16_t*> vMinusLine(vLen, 0, pWork, params.width*2);
	RingLinePtr<uint16_t*> vPlusLine(vLen, 0, pWork, params.width*2);

	// vTop collect
	for (size_t y=0; y<vLen; ++y) {
		const uint8_t* hMinus = hLine;
		const uint8_t* hPlus = hLine+hLen;
		size_t hSum = 0;
		// hLeft collect
		for (size_t x=0; x<hLen; ++x) {
			hSum += hLine[x];
		}
		// hCenter
		for (size_t x=hRad; x<hCount-hRad; ++x) {
			hSum -= *hMinus++;
			hSum += *hPlus++;
			vPlusLine[x] = hSum;
			vSumLine[x] += hSum;
		}
		// hRight
		;
		OffsetPtr(hLine, params.srcLineOffsetBytes);
		vPlusLine.moveNext();
	}

	// vMiddle
	for (size_t y=vRad; y<vCount-vRad; ++y) {

		const uint8_t* hMinus = hLine;
		const uint8_t* hPlus = hLine+hLen;
		size_t hSum = 0;
		// hLeft collect
		for (size_t x=0; x<hLen; ++x) {
			hSum += hLine[x];
		}
		// hCenter
		for (size_t x=hRad; x<hCount-hRad; ++x) {
			hSum -= *hMinus++;
			hSum += *hPlus++;
			
			// in this way, vPlus memory read is not required.
			// but memory access pattern is a bit complex.
			uint32_t vSum = vSumLine[x];
			vSum -= vMinusLine[x];
			vSum += hSum;
			vPlusLine[x] = hSum;
			vSumLine[x] = vSum;
			vLine[x] = (vSum * invLen) >> 24;
		}
		// hRight
		;
		OffsetPtr(hLine, params.srcLineOffsetBytes);
		OffsetPtr(vLine, params.dstLineOffsetBytes);
		vMinusLine.moveNext();
		vPlusLine.moveNext();
	}
}

