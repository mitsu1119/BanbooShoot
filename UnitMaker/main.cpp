#include "UnitMaker.h"

HINSTANCE hInst;

// Frame window class.
TCHAR szFrameClassName[] = _T("framewindow");

// Child window class.
TCHAR szChildDoc[] = _T("document");

// Menu
HMENU hMenuFirst, hMenuFirstWnd;

// gdiplus
Gdiplus::GdiplusStartupInput gpSI;
ULONG_PTR lpToken;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	
	Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);
	
	hInst = hInstance;

	if(!MyRegisterWC(FrameWndProc, szFrameClassName, (HBRUSH)(COLOR_APPWORKSPACE+1))) return 1;
	if(!MyRegisterWC(DocProc, szChildDoc, (HBRUSH)GetStockObject(WHITE_BRUSH))) return 1;

	// Setting menu bar in frame window.
	hMenuFirst = LoadMenu(hInst, MYMENU);
	hMenuFirstWnd = GetSubMenu(hMenuFirst, 0);

	HWND hFrame = CreateWindow(szFrameClassName, _T("UnitMaker"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, hMenuFirst, hInst, NULL);
	HWND hClient = GetWindow(hFrame, GW_CHILD);

	ShowWindow(hFrame, nCmdShow);
	UpdateWindow(hFrame);
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		if(!TranslateMDISysAccel(hClient, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DestroyMenu(hMenuFirst);

	Gdiplus::GdiplusShutdown(lpToken);

	return (int)msg.wParam;
}
