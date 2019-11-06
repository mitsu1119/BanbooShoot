#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const Image *img, double speed): speed(speed) {
	this->point = new Point(0, 0);
	this->anim = new Animation(img);
}

Character::~Character() {
	delete this->point;
	delete this->anim;
}

void Character::setCoord(double x, double y) {
	this->point->setX(x);
	this->point->setY(y);
}

void Character::draw() const {
	DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->anim->getHandle(), true);
}

// ----------------------------------------------------- Player class -----------------------------------------------------
Player::Player(const Image *img, double speed): Character(img, speed) {
}

void Player::move(Direction dir) {
	if(dir == CENTER) return;
	this->point->moveX(speed * cos(dir *M_PI / 4));
	this->point->moveY(-1 * speed * sin(dir * M_PI / 4));
}
