#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>
#include <windows.h>
#import <msxml6.dll>
#include "Util.h"
#include "Character.h"

// Base class
class Scene {
protected:
	int counter;

	Direction keyDirection;

	void checkKey();
	virtual void keyProcessing() = 0;

public:
	Scene(): counter(0), keyDirection(CENTER) {}

	virtual int update() = 0;
	virtual void draw() const = 0;
};

// Play class. Actual game disp.
constexpr size_t MAX_ENEMY_NUM = 10;
class Play: public Scene {
private:
	// Player datas.
	Player *player;

	// Enemy datas.
	enum PoolTuple {
		POOL_FLAG, POOL_BODY
	};
	std::vector<std::tuple<bool, Enemy *>> enemyPool;

	// System datas.
	void keyProcessing();
	ScreenRect screen;
	void enemyProcessing();

	// Image database.
	std::unordered_map<std::string, std::vector<const Image *>> enemyImages;
	std::unordered_map<std::string, Animation *> enemyAnimations;

	// Loading function.
	bool loadStage(std::string stagePath);

public:
	Play(std::string stagePath, Player *player, ScreenRect screen);
	~Play();

	int update();
	void draw() const;
};

// Scene manager.
class Game {
private:
	enum SceneType {
		SCENE_GAME_1
	};

	// Player datas.
	std::vector<Player *> player;

	// System datas.
	ScreenRect playScreen;
	SceneType nowSceneType;
	Scene *nowScene;

	// Image database.
	std::unordered_map<std::string, std::vector<const Image *>> playerImages;

	// Loading function.
	bool loadPlayers();

public:
	Game(ScreenRect playScreen);
	~Game();

	void update();
	void draw() const;
};
