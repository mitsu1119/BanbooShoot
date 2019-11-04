#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	if(DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		ClearDrawScreen();

		DrawString(0, 0, "Hello\n", GetColor(255, 255, 255));

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
