#pragma once
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
	Scene *nowScene;
	ScreenRect playScreen;

public:
	Game(ScreenRect playScreen);
	~Game();

	void update();
	void draw() const;
};
