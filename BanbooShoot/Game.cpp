#include "Game.h"

// ----------------------------------------------------- Play class -------------------------------------------------------
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

// ----------------------------------------------------- Game class -------------------------------------------------------
Game::Game() {
	this->player = new Character("dat\\image\\player\\Shirokami.png");

	// The first scene type is game play scene.
	this->nowScene = new Play(this->player);
}

Game::~Game() {
	delete this->player;
	delete this->nowScene;
}

void Game::update() {
	this->nowScene->update();
}

void Game::draw() {
	this->nowScene->draw();
}
