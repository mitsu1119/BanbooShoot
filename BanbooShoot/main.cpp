#include "DxLib.h"
#include "Character.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(true);
	if(DxLib_Init() == -1) return -1;

	Image image("dat\\image\\player\\Shirokami.png");
	Character chara(&image);

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		ClearDrawScreen();

		chara.draw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
