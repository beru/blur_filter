
#include "winmain2.h"
#include <windowsx.h>
#include <assert.h>

#include "stdint.h"
#include <vector>

#include <tchar.h>
#include <stdio.h>

#include "timer.h"
#include "winutil.h"

#include "ReadImage/ReadImage.h"
#include "ReadImage/File.h"

#include "blur/BoxBlur1b.h"

namespace {

HWND hWnd;
HBITMAP hBMP;
std::vector<uint8_t> bmiBuff(sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 256);
//BITMAPINFO bmi;
void* pBits;
HDC hMemDC;
size_t dstWidth;
int dstHeight;

ImageInfo imageInfo;
unsigned char* pImage;
unsigned char palettes[256 * 4];

int x_ = 0;
int y_ = 0;

const UINT_PTR TIMER_ID = 100;
const int TIMER_ELAPSE = 50;

void loadImage(const wchar_t* fileName)
{
	FILE* f = _wfopen(fileName, L"rb");
	if (!f) {
		printf("failed to open file : %s\n", fileName);
		return;
	}
	File fo(f);
	ReadImageInfo(fo, imageInfo);
	
	assert(imageInfo.bitsPerSample == 8 && imageInfo.samplesPerPixel == 1);
	const size_t size = imageInfo.width * imageInfo.height;
	pImage = (unsigned char*) _aligned_malloc(size, 64);
	ReadImageData(fo, pImage, imageInfo.width, palettes);
	fclose(f);

}

void render()
{
//	TRACE("%d ", x_);
	
	BoxBlur1bParams params;
	params.src = pImage;
	params.dst = (unsigned char*)pBits;
	params.srcLineOffsetBytes = imageInfo.width;
	params.dstLineOffsetBytes = dstWidth;
	params.width = min(dstWidth, imageInfo.width);
	params.height = min(dstHeight, imageInfo.height);
#if 0
	params.hRadius = x_;
	params.vRadius = y_;
#else
	params.hRadius = x_ * 2;
	params.vRadius = y_ * 2;
#endif
	params.subpixelRadius = false;
	params.quality = 0;
	
	BoxBlur1b(params);
}

} // anonymous namespace



void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	::SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, 0);
	
	BITMAPINFO* pBMI = (BITMAPINFO*) &bmiBuff[0];
	BITMAPINFO& bmi = *pBMI;
	
	dstWidth = 1920;
	dstHeight = 1080;
	
#if 1
	RGBQUAD rgb;
	for (size_t i=0; i<256; ++i) {
		rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = i;
		bmi.bmiColors[i] = rgb;
	}
	hBMP = CreateDIB(dstWidth, -dstHeight, 8, bmi, pBits);
#else
	int bitsPerPixel = 32;
	hBMP = CreateDIB(dstWidth, -dstHeight, bitsPerPixel, bmi, pBits);
#endif
	
	HDC hWndDC = ::GetDC(hWnd);
	hMemDC = ::CreateCompatibleDC(hWndDC);
	::SetMapMode(hMemDC, ::GetMapMode(hWndDC));
	::ReleaseDC(hWnd, hWndDC);
	::SelectObject(hMemDC, hBMP);
	
	int argc;
	wchar_t** argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argc >= 2) {
		loadImage(argv[1]);
	}
}

void OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	::DeleteDC(hMemDC);
	::DeleteObject(hBMP);
}

void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);

	Timer timer;
	
	render();
	
	double elapsed = timer.ElapsedSecond() * 1000;
	
	TCHAR str[32];
	_stprintf(str, _T("%f"), elapsed);
	::SetTextColor(hMemDC, -1);
	::TextOut(hMemDC, 10, 10, str, _tcslen(str));
	
	const RECT& rec = ps.rcPaint;
	::BitBlt(
		hdc,
		rec.left,
		rec.top,
		rec.right - rec.left,
		rec.bottom - rec.top,
		hMemDC,
		rec.left,
		rec.top,
		SRCCOPY
	);
	
	EndPaint(hWnd, &ps);
}

void OnMouseDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
//	if (wParam & MK_LBUTTON) {
//	}
}

void OnMouseUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
//	if (wParam & MK_LBUTTON) {
//	}
}

void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
//	if (wParam & MK_LBUTTON) {
//	}
	x_ = GET_X_LPARAM(lParam);
	y_ = GET_Y_LPARAM(lParam);
}

void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (!IsWindow(hWnd) || wParam != TIMER_ID) {
		return;
	}
	::InvalidateRect(hWnd, NULL, FALSE);
}

