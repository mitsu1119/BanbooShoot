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
Play::Play(std::string stagePath, Player *player, ScreenRect screen): Scene(), pShotFlag(false), player(player), enemyCounter(0), screen(screen) {
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

	// ルートタグを探索
	MITXMLElement *pRoot = pDocument.selectRootNode();

	// 敵画像データのロード
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

	// ステージデータの作成
	// "this->stage" の敵データは出現タイミングの順でソートされている必要がある
	MITXMLElement *pEnemy;
	double speed;
	int timing, x, y;
	pList = pRoot->selectNodes("enemy");
	this->stage.resize(pList.length());
	for(size_t i = 0; i < pList.length(); i++) {
		pEnemy = pList.item[i];
		name = pEnemy->getAttribute("name");
		speed = std::stof(pEnemy->getAttribute("speed"));
		timing = std::stoi(pEnemy->getAttribute("timing"));
		x = std::stoi(pEnemy->getAttribute("x"));
		y = std::stoi(pEnemy->getAttribute("y"));
		this->stage[i] = {name, x, y, speed, timing, MovingPath(pEnemy->getAttribute("path"))};
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

void Play::checkKey() {
	Scene::checkKey();

	int padkey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if(padkey & PAD_INPUT_1) this->pShotFlag = true;
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

	if(this->pShotFlag) this->player->shot(this->playerShotPool);
}

void Play::enemyProcessing() {
	// 敵のフラグ処理
	size_t newindex;
	std::string enemyName;
	for(size_t i = this->enemyCounter; i < this->stage.size(); i++) {
		// 敵の出現タイミングが来た時、"this->stage" からデータをロードし出現させる
		if(this->counter == std::get<STG_TIMING>(this->stage[i])) {
			if(this->falsePoolIndex.size() == 0) break;
			enemyName = std::get<STG_NAME>(this->stage[i]);
			newindex = this->falsePoolIndex.front();
			this->falsePoolIndex.pop_front();
			new(std::get<POOL_BODY>(this->enemyPool[newindex])) Enemy(*this->enemyAnimations[enemyName]->getParts(), this->enemyAnimations[enemyName]->getInterval(), std::get<STG_X>(this->stage[i]), std::get<STG_Y>(this->stage[i]), std::get<STG_SPEED>(this->stage[i]), std::move(std::get<STG_MOVEPATH>(this->stage[i])));
			std::get<POOL_FLAG>(this->enemyPool[newindex]) = true;
			this->enemyCounter++;
		} else if(this->counter > std::get<STG_TIMING>(this->stage[i])) {
			break;
		}
	}

	// 出現中の敵の更新処理
	for(auto &i: this->enemyPool) {
		if(std::get<POOL_FLAG>(i)) std::get<POOL_BODY>(i)->update();
	}
}

void Play::draw() const {
	ClearDrawScreen();

	this->player->draw();
	for(auto &i: this->playerShotPool) {
		if(std::get<POOL_FLAG>(i)) DrawGraph(0, 0, std::get<POOL_BODY>(i)->getHandle(), true);
	}
	for(auto &i: this->enemyPool) {
		if(std::get<POOL_FLAG>(i)) std::get<POOL_BODY>(i)->draw();
	}

	ScreenFlip();
}

// ----------------------------------------------------- Game class -------------------------------------------------------
Game::Game(ScreenRect playScreen): playScreen(playScreen) {
	loadShots();
	loadPlayers();
	this->nowSceneType = SCENE_GAME_1;

	// とりあえず player[0] を使用することにする
	this->nowScene = new Play("dat\\stage\\stage1.xml", this->player[0], playScreen);
}

Game::~Game() {
	for(auto &i: this->player) delete i;
	delete this->nowScene;

	for(auto &myPair: this->shotImages) delete myPair.second;
	for(auto &myPair: this->playerImages) {
		for(auto &image: myPair.second)
			delete image;
	}
}

bool Game::loadShots() {
	MITXMLDocument pDocument("dat\\database\\shot.xml");

	// ルートタグの探索
	MITXMLElement *pRoot = pDocument.selectRootNode();

	// 弾画像データのロード
	std::string name, path;
	MITXMLElement *pShotImage;
	MITXMLNodeList pList = pRoot->selectNodes("shotimage");
	for(size_t i = 0; i < pList.length(); i++) {
		pShotImage = pList.item[i];
		name = pShotImage->getAttribute("name");
		path = pShotImage->getAttribute("path");
		this->shotImages[name] = new Image(path.c_str());
	}

	return true;
}

bool Game::loadPlayers() {
	MITXMLDocument pDocument("dat\\database\\player.xml");

	// ルートタグの探索
	MITXMLElement *pRoot = pDocument.selectRootNode();

	// 自機画像データのロード
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

	// 自機定義をロード
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

		// とりあえずバリスタショット
		this->player.emplace_back(new Player(this->playerImages[name], this->playerImages[leftName], this->playerImages[rightName], playerImagesInterval[name], this->playScreen.getRightX() / 2, this->playScreen.getBottomY() - 60, speed, this->shotImages["varistor"]));
	}
	return true;
}

void Game::update() {
	this->nowScene->update();
}

void Game::draw() const {
	this->nowScene->draw();
}
