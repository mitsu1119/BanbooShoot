#include "Character.h"

// ----------------------------------------------------- Base class -------------------------------------------------------
Character::Character(const Image *img): img(img) {
	this->point = new Point(0, 0);
}

Character::~Character() {
	delete this->point;
}
