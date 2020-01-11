#import "C:\Windows\System32\Macromed\Flash\Flash.ocx" rename_namespace("Flash")

#include <atlbase.h>
#include <atlwin.h>
#include <WinInet.h>
#include <string.h>
#include <Windows.h>

#pragma comment(lib, "wininet")

#define WIN_W 600
#define WIN_H 550

CComModule _ccm;
HINSTANCE ghInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	static Flash::IShockwaveFlash* flash_o;
	static HWND hWndAX;
	static RECT rc;

	switch (uMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rc);
		hWndAX = CreateWindowEx(0, CAxWindow2::GetWndClassName(), NULL, WS_CHILD | WS_VISIBLE, 0, 0, WIN_W, WIN_H,
			hWnd, NULL, ghInst, NULL);

		if (FAILED(CoCreateInstance(__uuidof(Flash::ShockwaveFlash), NULL, CLSCTX_ALL,
			__uuidof(Flash::IShockwaveFlash), (void**)& flash_o)))
			return -1;

		if (FAILED(AtlAxAttachControl(flash_o, hWndAX, NULL))) {
			flash_o->Release();
			return -1;
		}
		InternetSetCookie(TEXT("https://{server}.darkorbit.com/"), NULL, TEXT("dosid = sid"));
		flash_o->Base = "https://darkorbit-22.bpsecure.com/";
		flash_o->FlashVars = "params";
			flash_o->Movie = "preloader";

		GetWindowRect(hWnd, &rc);

		SetWindowPos(hWnd, nullptr, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		return 0;

	case WM_COMMAND:
		return 0;
	case WM_SIZE:
		MoveWindow(hWndAX, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWndAX);
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		flash_o->Release();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc = { 0 };
	ghInst = hInstance;

	CoInitialize(NULL);
	AtlAxWinInit();

	wc.lpszClassName = L"Dark";
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);


	if (!RegisterClass(&wc))return MessageBox(HWND_DESKTOP, L"RegisterClass", L"Error", MB_ICONERROR | MB_OK);

	HWND hWnd = CreateWindow(wc.lpszClassName, L"Dark", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, WIN_W, WIN_H, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return MessageBox(HWND_DESKTOP, L"CreateWindow", L"Error", MB_ICONERROR | MB_OK);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
