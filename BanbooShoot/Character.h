#pragma once
#include "Util.h"

// Base class.
class Character {
protected:
	const Image *img;
	Point *point;

	Character(const Image *img);
	~Character();

public:
	const Point *getPoint() const;
	const Image *getImage() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->img; }
