#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const Image *img, double speed): img(img), speed(speed), imgFreeFlag(false) {
	this->point = new Point(0, 0);
}

Character::Character(const char *imgpath, double speed): speed(speed), imgFreeFlag(true) {
	this->img = new Image(imgpath);
	this->point = new Point(0, 0);
}

Character::~Character() {
	delete this->point;
	if(imgFreeFlag) delete this->img;
}

void Character::setCoord(double x, double y) {
	this->point->setX(x);
	this->point->setY(y);
}

void Character::draw() const {
	DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->img->getHandle(), true);
}

// ----------------------------------------------------- Player class -----------------------------------------------------
Player::Player(const Image *img, double speed): Character(img, speed) {
}

Player::Player(const char *imgpath, double speed): Character(imgpath, speed) {
}

void Player::move(Direction dir) {
	if(dir == CENTER) return;
	this->point->moveX(speed * cos(dir *M_PI / 4));
	this->point->moveY(-1 * speed * sin(dir * M_PI / 4));
}
