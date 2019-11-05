#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const char *imgpath) {
	this->img = new Image(imgpath);
	this->point = new Point(0, 0);
}

Character::~Character() {
	delete this->point;
}

void Character::setCoord(double x, double y) {
	this->point->setX(x);
	this->point->setY(y);
}

void Character::draw() const {
	DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->img->getHandle(), true);
}

// ----------------------------------------------------- Player class -----------------------------------------------------
Player::Player(const char *imgpath): Character(imgpath) {
}

void Player::move(Direction dir) {
	if(dir == CENTER) return;
	this->point->moveX(cos(dir *M_PI / 4));
	this->point->moveY(-1 * sin(dir * M_PI / 4));
}
