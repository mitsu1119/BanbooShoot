#include "DxLib.h"
#include "Game.h"
#include "Util.h"
#include "Character.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(true);
	if(DxLib_Init() == -1) return -1;

	Image image("dat\\image\\player\\Shirokami.png");
	Character chara(&image);

	Play game(&chara);

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		game.update();
		game.draw();
	}

	DxLib_End();
	return 0;
}
