
#include <stdio.h>

#include "ReadImage/ReadImage.h"
#include "ReadImage/File.h"

#include <malloc.h>

void transpose(
	const unsigned char* pSrc,
	unsigned char* pDst,
	size_t width, size_t height,
	int srcLineOffset,
	int dstLineOffset
	)
{
	const unsigned char* pSrcLine = pSrc;
	unsigned char* pDstLine = pDst;
	for (size_t y=0; y<height; ++y) {
		unsigned char* pDstLine2 = pDstLine;
		for (size_t x=0; x<width; ++x) {
			*pDstLine2 = pSrcLine[x];
			OffsetPtr(pDstLine2, dstLineOffset);
		}
		OffsetPtr(pSrcLine, srcLineOffset);
		++pDstLine;
	}
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("specify filename\n");
		return 1;
	}
	
	FILE* f = fopen(argv[1], "rb");
	if (!f) {
		printf("failed to open file : %s\n", argv[1]);
		return 1;
	}
	File fo(f);
	ImageInfo imageInfo;
	ReadImageInfo(fo, imageInfo);
	
	size_t width = imageInfo.width;
	size_t height = imageInfo.height;
	assert(imageInfo.bitsPerSample == 8 && imageInfo.samplesPerPixel == 1);
	const size_t size = width * height;
	unsigned char* pSrc = (unsigned char*) _aligned_malloc(size, 64);
	unsigned char palettes[256 * 4];
	ReadImageData(fo, pSrc, width, palettes);
	fclose(f);
	
	for (size_t i=0; i<size; ++i) {
		pSrc[i] = palettes[4 * pSrc[i]];
	}

	unsigned char* pSrc2 = (unsigned char*) _aligned_malloc(size, 64);
	transpose(pSrc, pSrc2, width, height, width, width);
	
	return 0;
}

