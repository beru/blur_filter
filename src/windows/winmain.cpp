
#include <windows.h>
#include <tchar.h>

#include "winmain2.h"

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		OnCreate(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy(hWnd, wParam, lParam);
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_PAINT:
		OnPaint(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
//		::SetCapture(hWnd);
		OnMouseDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
//		if (::GetCapture() == hWnd) {
//			::ReleaseCapture();
			OnMouseUp(hWnd, wParam, lParam);
//		}
		break;
	case WM_MOUSEMOVE:
//		if (::GetCapture() == hWnd) {
			OnMouseMove(hWnd, wParam, lParam);
//		}
		break;
	case WM_TIMER:
		OnTimer(hWnd, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static const wchar_t* WINDOW_CLASS_NAME = L"TEST_CLASS";
static const wchar_t* WINDOW_TITLE = L"TEST";

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE,
	   WS_OVERLAPPED
	   |WS_CLIPCHILDREN
	   |WS_CLIPSIBLINGS
	   |WS_CAPTION
	   |WS_SYSMENU
	   |WS_MINIMIZEBOX
	   |WS_MAXIMIZEBOX
	   |WS_THICKFRAME
	   ,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
	
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground	= 0; //(HBRUSH)(COLOR_BACKGROUND+4);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;
	
	// �O���[�o������������������Ă��܂��B
	MyRegisterClass(hInstance);
	
	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	// ���C�� ���b�Z�[�W ���[�v:
	while (1) {
        BOOL ret = ::GetMessage(&msg, NULL, 0, 0);
		if (ret == 0/*WM_QUIT*/ || ret == -1/*ERROR*/) {
			if (ret == -1) {
				DWORD err = ::GetLastError();
				assert(false);
			}
            break;
		}
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}
