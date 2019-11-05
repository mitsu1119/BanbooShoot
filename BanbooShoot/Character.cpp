#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const Image *img): img(img) {
	this->point = new Point(0, 0);
}

Character::Character(const char *imgpath) {
	this->img = new Image(imgpath);
	this->point = new Point(0, 0);
}

Character::~Character() {
	delete this->point;
}

void Character::draw() const {
	DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->img->getHandle(), true);
}
