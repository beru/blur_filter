
#include "BoxBlur1b.h"

extern void BoxBlur1b_1stOrder(const BoxBlur1bParams& params);

void BoxBlur1b(const BoxBlur1bParams& params)
{
	if (params.subpixelRadius) {
		switch (params.quality) {
		case 0:
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
			break;
		case 2:
			break;
		}
	}
}

