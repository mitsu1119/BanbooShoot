#pragma once
#include "resource.h"
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <gdiplus.h>
#include <vector>
#include "..//BanbooShoot/Util.h"

#define IDM_FIRSTCHILD 0x100
#define MYMENU MAKEINTRESOURCE(IDR_MENU1)
#define MDICLIENT _T("MDICLIENT")
#define EDIT _T("EDIT")

extern TCHAR szFrameClassName[];
extern TCHAR szChildDoc[], szSPList[];
extern HINSTANCE hInst;
extern HMENU hMenuFirst, hMenuFirstWnd;
extern HMENU hMenuFirst, hMenuFirstWnd;

// Split list structure.
// pair: Start point, End point.
typedef std::vector<std::pair<Point, Point>> SpList;

// Document data.
struct DocData {
	SpList *splits;
	Gdiplus::Image *img;
};

// Open selected file and store to GetWindowLongPtr(hWnd, GWLP_USERDATA).
// If @szFileName is not nullptr, @szFileName receives a copy of szFileName.
Gdiplus::Image *OpenFileDialog(HWND hWnd, TCHAR *szFileName = nullptr, DWORD copySize = 0) {
	OPENFILENAME ofn = {0};
	TCHAR szFilePath[MAX_PATH] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = TEXT("Image files {*.png}\0*.png\0");
	ofn.lpstrFile = szFilePath;
	ofn.nMaxFile = MAX_PATH;
	if(szFileName != nullptr) {
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = copySize;
	}
	ofn.Flags = OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);

	return new Gdiplus::Image(szFilePath);
}

// Set window (@hWnd) size based on client window size.
BOOL SetClientSize(HWND hWnd, size_t width, size_t height) {
	RECT rw, rc;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	size_t nw = (rw.right - rw.left) - (rc.right - rc.left) + width;
	size_t nh = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return SetWindowPos(hWnd, NULL, 0, 0, nw, nh, SWP_NOMOVE | SWP_NOZORDER);
}

BOOL CALLBACK CloseAllProc(HWND hWnd, LPARAM lp) {
	SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
	return TRUE;
}

BOOL CALLBACK CloseAllDoc(HWND hWnd, LPARAM lp) {
	TCHAR className[MAX_PATH];
	GetClassName(hWnd, className, MAX_PATH);
	if(wcsncmp(className, szSPList, MAX_PATH)  != 0) SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
	return TRUE;
}

void onPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Gdiplus::Graphics g(hdc);

	Gdiplus::Image *img = (Gdiplus::Image *)((DocData *)(GetWindowLongPtr(hWnd, GWLP_USERDATA)))->img;

	g.DrawImage(img, 0, 0, img->GetWidth(), img->GetHeight());

	EndPaint(hWnd, &ps);
}

// Main frame.
LRESULT CALLBACK SPListProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
int MyRegisterWC(WNDPROC WndProc, LPCTSTR ClassName, HBRUSH BackGround);
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	static HWND hClient, SPList;
	static HMENU hMenu;
	MENUITEMINFO menuInfo;
	HWND hChild;
	CLIENTCREATESTRUCT ccs;
	MDICREATESTRUCT mdic;

	switch(msg) {
	case WM_CREATE:
		// Menu
		hMenu = GetMenu(hWnd);

		// Client
		ccs.hWindowMenu = hMenuFirstWnd;
		ccs.idFirstChild = IDM_FIRSTCHILD;
		hClient = CreateWindow(MDICLIENT, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 500, 500, hWnd, (HMENU)1, hInst, &ccs);

		// SPList
		mdic.szClass = szSPList;
		mdic.szTitle = _T("SPList");
		mdic.hOwner = hInst;
		mdic.x = mdic.y = mdic.cx = mdic.cy = CW_USEDEFAULT;
		mdic.style = 0;
		mdic.lParam = 0;
		SPList = (HWND)SendMessage(hClient, WM_MDICREATE, 0, (LPARAM)&mdic);
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
		case IDM_CLOSEALLDOC:
			EnumChildWindows(hClient, &CloseAllDoc, 0);
			return 0;
		case IDM_CLOSEALL:
			EnumChildWindows(hClient, &CloseAllProc, 0);
			return 0;
		case IDM_SPLITLIST:
			menuInfo.cbSize = sizeof MENUITEMINFO;
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(hMenu, IDM_SPLITLIST, FALSE, &menuInfo);
			if(menuInfo.fState == MFS_CHECKED) {
				// Hide splist list window.
				ShowWindow(SPList, SW_HIDE);
				UpdateWindow(SPList);
				menuInfo.fState = MFS_UNCHECKED;
			} else {
				// Show splist list window.
				ShowWindow(SPList, SW_SHOW);
				UpdateWindow(SPList);
				menuInfo.fState = MFS_CHECKED;
			}
			SetMenuItemInfo(hMenu, IDM_SPLITLIST, FALSE, &menuInfo);
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
	DocData *docd;

	switch(msg) {
	case WM_CREATE:
	{
		TCHAR szFileName[MAX_PATH];
		docd = new DocData;
		docd->img = OpenFileDialog(hWnd, szFileName, MAX_PATH);
		docd->splits = new SpList;
		docd->splits->push_back(std::make_pair(Point(0, 0), Point((double)docd->img->GetWidth(), (double)docd->img->GetHeight())));
		SetWindowText(hWnd, szFileName);
		SetClientSize(hWnd, docd->img->GetWidth(), docd->img->GetHeight());
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)docd);
		break;
	}
	case WM_PAINT: 
		onPaint(hWnd);
		return 0;
	case WM_MDIDESTROY:
		docd = (DocData *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(docd->img != nullptr) delete docd->img;
		if(docd->splits != nullptr) delete docd->splits;
		delete docd;
		break;
	}
	return DefMDIChildProc(hWnd, msg, wp, lp);
}

// Split list window. Layer palette.
LRESULT CALLBACK SPListProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
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
