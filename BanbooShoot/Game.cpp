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
Play::Play(std::string stagePath, Player *player, ScreenRect screen): Scene(), player(player), enemyCounter(0), screen(screen) {
	this->enemyPool.resize(MAX_ENEMY_NUM);
	this->falsePoolIndex.resize(MAX_ENEMY_NUM);
	std::iota(this->falsePoolIndex.begin(), this->falsePoolIndex.end(), 0);
	for(size_t i = 0; i < MAX_ENEMY_NUM; i++) {
			this->enemyPool[i] = {false, (Enemy *)std::malloc(sizeof(Enemy))};
	}
	loadStage(stagePath);
}

Play::~Play() {
	for(auto &myPair: this->enemyImages) {
		for(auto &image: myPair.second)
			delete image;
	}
	
	for(auto &anim: this->enemyAnimations) {
		delete anim.second;
	}
}

bool Play::loadStage(std::string stagePath) {
	MITXMLDocument pDocument(stagePath.c_str());

	// Search root tag.
	MITXMLElement *pRoot = pDocument.selectRootNode();

	// Load enemyimages.
	std::string name, path;
	MITXMLNodeList pList = pRoot->selectNodes("enemyimage");

	MITXMLElement *pEnemyImage, *pAnimation;
	std::string value, buf;
	int sx, sy, width, height, enemyImageInterval, err;
	for(size_t i = 0; i < pList.length(); i++) {
		pEnemyImage = pList.item[i];
		name = pEnemyImage->getAttribute("name");
		path = pEnemyImage->getAttribute("path");
		
		buf = pEnemyImage->getAttribute("animinterval", &err);
		if(err != E_NULL) enemyImageInterval = std::stoi(buf);
		else enemyImageInterval = 100;

		MITXMLNodeList pAnimations = pEnemyImage->selectNodes("animation");
		if(pAnimations.length() != 0) {
			for(size_t j = 0; j < pAnimations.length(); j++) {
				pAnimation = pAnimations.item[j];
				sx = std::stoi(pAnimation->getAttribute("sx"));
				sy = std::stoi(pAnimation->getAttribute("sy"));
				width = std::stoi(pAnimation->getAttribute("width"));
				height = std::stoi(pAnimation->getAttribute("height"));
				this->enemyImages[name].emplace_back(new Image(path.c_str(), sx, sy, width, height));
			}
		} else {
			this->enemyImages[name].emplace_back(new Image(path.c_str()));
		}
		this->enemyAnimations[name] = new Animation(this->enemyImages[name], enemyImageInterval);
	}

	MITXMLElement *pEnemy;
	double speed;
	int timing;
	pList = pRoot->selectNodes("enemy");
	this->stage.resize(pList.length());
	for(size_t i = 0; i < pList.length(); i++) {
		pEnemy = pList.item[i];
		name = pEnemy->getAttribute("name");
		speed = std::stof(pEnemy->getAttribute("speed"));
		timing = std::stoi(pEnemy->getAttribute("timing"));
		this->stage[i] = {name, speed, timing};
	}

	return true;
}

int Play::update() {
	this->counter++;
	checkKey();
	keyProcessing();
	this->player->update();
	enemyProcessing();
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

void Play::enemyProcessing() {
	// Enemy flag processings.
	size_t newindex;
	for(size_t i = this->enemyCounter; i < this->stage.size(); i++) {
		if(this->counter == std::get<STG_TIMING>(this->stage[i])) {
			if(this->falsePoolIndex.size() == 0) break;
			newindex = this->falsePoolIndex.front();
			this->falsePoolIndex.pop_front();
			new(std::get<POOL_BODY>(this->enemyPool[newindex])) Enemy(*this->enemyAnimations["Sphere"]->getParts(), this->enemyAnimations["Sphere"]->getInterval(), 3);
			std::get<POOL_FLAG>(this->enemyPool[newindex]) = true;
			this->enemyCounter++;
		} else if(this->counter > std::get<STG_TIMING>(this->stage[i])) {
			break;
		}
	}

	// Enemys' update.
	for(auto &i: this->enemyPool) {
		if(std::get<POOL_FLAG>(i)) std::get<POOL_BODY>(i)->update();
	}
}

void Play::draw() const {
	ClearDrawScreen();

	this->player->draw();
	for(auto &i: this->enemyPool) {
		if(std::get<POOL_FLAG>(i)) std::get<POOL_BODY>(i)->draw();
	}

	ScreenFlip();
}

// ----------------------------------------------------- Game class -------------------------------------------------------
Game::Game(ScreenRect playScreen): playScreen(playScreen) {
	loadPlayers();

	// The first scene type is play
	this->nowSceneType = SCENE_GAME_1;

	// Use player 0. 
	this->nowScene = new Play("dat\\stage\\stage1.xml", this->player[0], playScreen);
}

Game::~Game() {
	for(auto &i: this->player) delete i;
	delete this->nowScene;
	for(auto &myPair: this->playerImages) {
		for(auto &image: myPair.second)
			delete image;
	}
}

bool Game::loadPlayers() {
	MITXMLDocument pDocument("dat\\database\\player.xml");

	// Search root tag.
	MITXMLElement *pRoot = pDocument.selectRootNode();

	// Load playerimages.
	std::string name, path, buf;
	int sx, sy, width, height, err;
	std::unordered_map<std::string, int> playerImagesInterval;
	MITXMLElement *pPlayerImage, *pAnimation;
	MITXMLNodeList pList = pRoot->selectNodes("playerimage");
	for(size_t i = 0; i < pList.length(); i++) {
		pPlayerImage = pList.item[i];
		name = pPlayerImage->getAttribute("name");
		path = pPlayerImage->getAttribute("path");

		buf = pPlayerImage->getAttribute("animinterval", &err);
		if(err != E_NULL) playerImagesInterval[name] = std::stoi(buf);
		else playerImagesInterval[name] = 100;
		
		MITXMLNodeList pAnimations = pPlayerImage->selectNodes("animation");
		if(pAnimations.length() != 0) {
			for(size_t j = 0; j < pAnimations.length(); j++) {
				pAnimation = pAnimations.item[j];
				sx = std::stoi(pAnimation->getAttribute("sx"));
				sy = std::stoi(pAnimation->getAttribute("sy"));
				width = std::stoi(pAnimation->getAttribute("width"));
				height = std::stoi(pAnimation->getAttribute("height"));
				this->playerImages[name].emplace_back(new Image(path.c_str(), sx, sy, width, height));
			}
		} else {
			this->playerImages[name].emplace_back(new Image(path.c_str()));
		}
	}

	// Load playerdefines.
	std::string leftName, rightName;
	double speed;
	MITXMLElement *pPlayerDefine;
	pList = pRoot->selectNodes("playerdefine");
	for(size_t i = 0; i < pList.length(); i++) {
		pPlayerDefine = pList.item[i];
		name = pPlayerDefine->getAttribute("name");
		speed = std::stof(pPlayerDefine->getAttribute("speed"));

		buf = pPlayerDefine->getAttribute("leftname", &err);
		if(err != E_NULL) leftName = buf;
		else leftName = name;

		buf = pPlayerDefine->getAttribute("rightname", &err);
		if(err != E_NULL) rightName = buf;
		else rightName = name;

		this->player.emplace_back(new Player(this->playerImages[name], this->playerImages[leftName], this->playerImages[rightName], playerImagesInterval[name], speed));
	}
	return true;
}

void Game::update() {
	this->nowScene->update();
}

void Game::draw() const {
	this->nowScene->draw();
}
