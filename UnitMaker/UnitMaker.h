#pragma once
#include "resource.h"
#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>

#define ID_CHILDWND 50001
#define TITLE _T("UnitMaker")
#define IDTEXT_MAINWND _T("UnitMaker")
#define IDTEXT_MDICLIENTWND _T("MDIClient")
#define IDTEXT_CHILD _T("MDIChild")

HINSTANCE hInst;

// Processing messages.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	CLIENTCREATESTRUCT ccsClient;
	static HWND hClientWindow;

	switch(message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		ccsClient.hWindowMenu = NULL;
		ccsClient.idFirstChild = ID_CHILDWND;
		hClientWindow = CreateWindow(IDTEXT_MDICLIENTWND, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)1, hInst, &ccsClient);
		break;
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case ID_FILE_NEW:
			CreateMDIWindow(IDTEXT_CHILD, _T("New"), 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hClientWindow, hInst, 0);
			break;
		case ID_FILE_OPEN:
			CreateMDIWindow(IDTEXT_CHILD, _T("Open"), 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hClientWindow, hInst, 0);
			break;
		}
		return 0;
	}
	return DefFrameProc(hWnd, hClientWindow, message, wParam, lParam);
}
