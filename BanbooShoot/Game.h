#pragma once
#include <unordered_map>
#include <string>
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
class Play: public Scene {
private:
	// Player datas.
	Player *player;

	// System datas.
	void keyProcessing();
	ScreenRect screen;

public:
	Play(Player *player, ScreenRect screen);

	int update();
	void draw() const;
};

// Scene manager.
class Game {
private:
	// Player datas.
	Player *player;

	// System datas.
	ScreenRect playScreen;
	Scene *nowScene;

	// Image database.
	std::unordered_map<std::string, const Image *> playerImages;

	// Loading function.
	bool loadPlayers();

public:
	Game(ScreenRect playScreen);
	~Game();

	void update();
	void draw() const;
};
