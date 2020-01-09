#pragma once
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <deque>
#include <windows.h>
#include "Util.h"
#include "Character.h"
#include "MovingPath.h"
#include "mitXML.h"
#include "MovingPath.h"

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

// Stage type.
enum StagePartTupleTag {
	STG_NAME, STG_X, STG_Y, STG_SPEED, STG_TIMING, STG_MOVEPATH
};
typedef std::tuple<std::string, int, int, double, int, MovingPath> StagePart;
typedef std::vector<StagePart> Stage;

// Play class. Actual game disp.
constexpr size_t MAX_ENEMY_NUM = 10;
class Play: public Scene {
private:
	// Player datas.
	Player *player;

	// Enemy datas.
	enum PoolTupleTag {
		POOL_FLAG, POOL_BODY
	};
	std::vector<std::tuple<bool, Enemy *>> enemyPool;
	std::deque<size_t> falsePoolIndex;		// List of indexes for "this->enemyPool" with the flag is false.
	Stage stage;
	size_t enemyCounter;

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
