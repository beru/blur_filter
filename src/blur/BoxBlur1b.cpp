
#include "BoxBlur1b.h"
#include "SubPixel_BoxBlur1b.h"

extern void BoxBlur1b_1stOrder(const BoxBlur1bParams& params);
extern void BoxBlur1b_2ndOrder(const BoxBlur1bParams& params);

void BoxBlur1b(const BoxBlur1bParams& params)
{
	if (0 && params.subpixelRadius) {
		switch (params.quality) {
		case 0:
			SubPixel_BoxBlur1b_1stOrder(params);
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}else {
		switch (params.quality) {
		case 0:
			BoxBlur1b_1stOrder(params);
			break;
		case 1:
			BoxBlur1b_2ndOrder(params);
			break;
		case 2:
			break;
		}
	}
}

