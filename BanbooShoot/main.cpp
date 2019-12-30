#include "DxLib.h"
#include "Game.h"
#include "Util.h"
#include "mitXML.h"

// XML test
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(true);
	SetGraphMode(540, 780, 32);
	if(DxLib_Init() == -1) return -1;

	MITXMLDocument *root;
	root = new MITXMLDocument("test.xml");

	MITXMLElement *pRoot = root->selectRootNode();
	std::string elem = pRoot->getAttribute("elem");
	std::string elem2 = pRoot->getAttribute("elem2");

	delete root;
	DxLib_End();
	return 0;
}
/*
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(true);
	SetGraphMode(540, 780, 32);
	if(DxLib_Init() == -1) return -1;

	Game game(ScreenRect(0, 0, 540, 780));

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		game.update();
		game.draw();
	}

	DxLib_End();
	return 0;
}
*/
