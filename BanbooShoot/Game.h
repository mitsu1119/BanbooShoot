#pragma once
#include "Util.h"
#include "Character.h"

// Base class
class Scene {
protected:
	int counter;

public:
	Scene(): counter(0) {}

	virtual int update() = 0;
	virtual void draw() = 0;
};

class Play: public Scene {
private:
	Character *player;

public:
	Play(Character *player);

	virtual int update();
	virtual void draw();
};

