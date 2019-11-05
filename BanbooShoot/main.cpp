#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(true);
	if(DxLib_Init() == -1) return -1;

	Game game;

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		game.update();
		game.draw();
	}

	DxLib_End();
	return 0;
}
