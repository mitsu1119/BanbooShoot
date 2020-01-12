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
		case IDM_FILE_NEW:
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
		default:
			hChild = (HWND)SendMessage(hClient, WM_MDIGETACTIVE, 0, 0);
			if(IsWindow(hChild)) SendMessage(hChild, WM_COMMAND, wp, lp);
			break;
		}
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
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
	static HWND hClient, hFrame;
	HWND hEdit;
	RECT rc;

	switch(msg) {
	case WM_CREATE:
		hClient = GetParent(hWnd);
		hFrame = GetParent(hClient);
		GetClientRect(hWnd, &rc);
		hEdit = CreateWindow(EDIT, NULL, WS_VISIBLE | WS_CHILD | ES_WANTRETURN | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | WS_HSCROLL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hWnd, NULL, hInst, NULL);
		return 0;
	case WM_SIZE:
		GetClientRect(hWnd, &rc);
		hEdit = GetWindow(hWnd, GW_CHILD);
		MoveWindow(hEdit, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
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
