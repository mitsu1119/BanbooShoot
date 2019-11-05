#include "Game.h"

// ----------------------------------------------------- Scene class ------------------------------------------------------
void Scene::checkKey() {
	int padkey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if(padkey & PAD_INPUT_LEFT) {
		if(padkey & PAD_INPUT_DOWN) keyDirection = LDOWN;
		else if(padkey & PAD_INPUT_UP) keyDirection = LUP;
		else keyDirection = LEFT;
	} else if(padkey & PAD_INPUT_RIGHT) {
		if(padkey & PAD_INPUT_DOWN) keyDirection = RDOWN;
		else if(padkey & PAD_INPUT_UP) keyDirection = RUP;
		else keyDirection = RIGHT;
	} else if(padkey & PAD_INPUT_DOWN) {
		keyDirection = DOWN;
	} else if(padkey & PAD_INPUT_UP) {
		keyDirection = UP;
	} else {
		keyDirection = CENTER;
	}
}

// ----------------------------------------------------- Play class -------------------------------------------------------
Play::Play(Player *player, ScreenRect screen): Scene(), player(player), screen(screen) {
}

int Play::update() {
	this->counter++;
	checkKey();
	keyProcessing();
	return 0;
}

void Play::keyProcessing() {
	double harfX = this->player->getImage()->getSizeX() / 2.0;
	double harfY = this->player->getImage()->getSizeY() / 2.0;
	int leftX = this->screen.getLeftX(), topY = this->screen.getTopY(), rightX = this->screen.getRightX(), bottomY = this->screen.getBottomY();

	this->player->move(keyDirection);
	if(keyDirection != CENTER) {
		if(this->player->getPoint()->getX() - harfX / 2.0 < leftX) this->player->setCoord(leftX + harfX / 2.0, this->player->getPoint()->getY());
		else if(this->player->getPoint()->getX() + harfX / 2.0 > rightX) this->player->setCoord(rightX - harfX / 2.0, this->player->getPoint()->getY());
		if(this->player->getPoint()->getY() - harfY / 2.0 < topY) this->player->setCoord(this->player->getPoint()->getX(), topY+ harfY / 2.0);
		else if(this->player->getPoint()->getY() + harfY / 2.0 > bottomY) this->player->setCoord(this->player->getPoint()->getX(), bottomY - harfY / 2.0);
	}
}

void Play::draw() const {
	ClearDrawScreen();

	this->player->draw();

	ScreenFlip();
}

// ----------------------------------------------------- Game class -------------------------------------------------------
Game::Game(ScreenRect playScreen): playScreen(playScreen) {
	loadPlayers();

	// The first cene type is play
	this->nowScene = new Play(this->player, playScreen);
}

Game::~Game() {
	delete this->player;
	delete this->nowScene;
}

bool Game::loadPlayers() {
	MSXML2::IXMLDOMDocument2Ptr pDocument;
	HRESULT hr = pDocument.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr)) return false;

	pDocument->async = VARIANT_FALSE;
	pDocument->validateOnParse = VARIANT_FALSE;
	pDocument->resolveExternals = VARIANT_FALSE;
	pDocument->preserveWhiteSpace = VARIANT_FALSE;

	if(pDocument->load("dat\\database\\player.xml") == VARIANT_TRUE) {
		// Search root tag.
		MSXML2::IXMLDOMNodeListPtr pList = pDocument->selectNodes("player");
		MSXML2::IXMLDOMElementPtr pRoot;
		if(pList->length > 0) pRoot = pList->item[0];
		else return false;
			
		// Load playerimage.
		_variant_t v;
		std::string name, path;
		pList = pRoot->selectNodes("//playerimage");
		MSXML2::IXMLDOMElementPtr pPlayerImage;
		for(int i = 0; i < pList->length; i++) {
			pPlayerImage = pList->item[i];
			v = pPlayerImage->getAttribute("name");
			name = _com_util::ConvertBSTRToString(v.bstrVal);
			v = pPlayerImage->getAttribute("path");
			path = _com_util::ConvertBSTRToString(v.bstrVal);
			this->playerImages[name] = new Image(path.c_str());
		}

		// Load playerdefine.
		pList = pRoot->selectNodes("//playerdefine");
		MSXML2::IXMLDOMElementPtr pPlayerDefine;
		double speed;
		for(int i = 0; i < pList->length; i++) {
			pPlayerDefine = pList->item[i];
			v = pPlayerDefine->getAttribute("name");
			name = _com_util::ConvertBSTRToString(v.bstrVal);
			v = pPlayerDefine->getAttribute("speed");
			speed = _wtof(v.bstrVal);
			this->player = new Player(this->playerImages[name], speed);
		}			
	}
	return true;
}

void Game::update() {
	this->nowScene->update();
}

void Game::draw() const {
	this->nowScene->draw();
}
