#pragma once

struct BoxBlur1bParams
{
	const uint8_t* src;
	uint8_t* dst;
	
	int16_t srcLineOffsetBytes;
	int16_t dstLineOffsetBytes;

	uint16_t width;
	uint16_t height;
	
	uint16_t hRadius;
	uint16_t vRadius;
	
	bool subpixelRadius;
	uint8_t quality;
};

void BoxBlur1b(const BoxBlur1bParams& params);

