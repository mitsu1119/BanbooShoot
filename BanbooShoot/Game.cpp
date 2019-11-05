#include "Game.h"

Play::Play(Character *player): Scene(), player(player) {
}

int Play::update() {
	this->counter++;
	return 0;
}

void Play::draw() {
	ClearDrawScreen();

	this->player->draw();

	ScreenFlip();
}
