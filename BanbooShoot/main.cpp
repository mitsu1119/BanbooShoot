#include "DxLib.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
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
