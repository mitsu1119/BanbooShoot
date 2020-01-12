#pragma once
#include "resource.h"
#include <Windows.h>
#include <tchar.h>
#include <string>

#define IDM_FIRSTCHILD 0x100
#define MYMENU MAKEINTRESOURCE(IDR_MENU1)
#define MDICLIENT _T("MDICLIENT")
#define EDIT _T("EDIT")

extern TCHAR szFrameClassName[];
extern TCHAR szChildDoc[];
extern HINSTANCE hInst;
extern HMENU hMenuFirst, hMenuFirstWnd;
extern HMENU hMenuFirst, hMenuFirstWnd;

// Open selected file and store to GetWindowLongPtr(hWnd, GWLP_USERDATA).
void OpenFileDialog(HWND hWnd) {
	OPENFILENAME ofn = {0};
	TCHAR szFilePath[MAX_PATH] = {};
	TCHAR *szFileName = (TCHAR *)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = TEXT("Image files {*.png}\0*.png\0");
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szFileName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
}

BOOL CALLBACK CloseAllProc(HWND hWnd, LPARAM lp) {
	SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
	return TRUE;
}

// Main frame.
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	static HWND hClient;
	HWND hChild;
	CLIENTCREATESTRUCT ccs;
	MDICREATESTRUCT mdic;

	switch(msg) {
	case WM_CREATE:
		ccs.hWindowMenu = hMenuFirstWnd;
		ccs.idFirstChild = IDM_FIRSTCHILD;
		hClient = CreateWindow(MDICLIENT, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 0, 0, hWnd, (HMENU)1, hInst, &ccs);
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wp)) {
		case IDM_NEW:
			mdic.szClass = szChildDoc;
			mdic.szTitle = _T("Document");
			mdic.hOwner = hInst;
			mdic.x = CW_USEDEFAULT;
			mdic.y = CW_USEDEFAULT;
			mdic.cx = CW_USEDEFAULT;
			mdic.cy = CW_USEDEFAULT;
			mdic.style = 0;
			mdic.lParam = 0;
			hChild = (HWND)SendMessage(hClient, WM_MDICREATE, 0, (LPARAM)&mdic);
			return 0;
		case IDM_CLOSE:
			hChild = (HWND)SendMessage(hClient, WM_MDIGETACTIVE, 0, 0);
			if(hChild) SendMessage(hClient, WM_MDIDESTROY, (WPARAM)hChild, 0);
			return 0;
		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		case IDM_CLOSEALL:
			EnumChildWindows(hClient, &CloseAllProc, 0);
			return 0;
		default:
			hChild = (HWND)SendMessage(hClient, WM_MDIGETACTIVE, 0, 0);
			if(IsWindow(hChild)) SendMessage(hChild, WM_COMMAND, wp, lp);
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hWnd, WM_COMMAND, IDM_CLOSEALL, 0);
		if(GetWindow(hClient, GW_CHILD)) return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hWnd, hClient, msg, wp, lp);
}

// Child document window.
LRESULT CALLBACK DocProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	TCHAR *szFileName;

	HDC hdc;
	PAINTSTRUCT ps;

	switch(msg) {
	case WM_CREATE:
		szFileName = new TCHAR[MAX_PATH];
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)szFileName);
		OpenFileDialog(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		szFileName = (TCHAR *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		TextOut(hdc, 5, 5, szFileName, _tcslen(szFileName));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		break;
	}
	return DefMDIChildProc(hWnd, msg, wp, lp);
}

// Window class regist.
int MyRegisterWC(WNDPROC WndProc, LPCTSTR ClassName, HBRUSH BackGround) {
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = BackGround;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	return (RegisterClassEx(&wc));
}
