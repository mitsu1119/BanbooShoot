#pragma once
#include "Util.h"

// Base class.
class Character {
protected:
	const Image *img;
	Point *point;				/* central coordinate */

public:
	Character(const Image *img);		/* generate from image */
	~Character();

	const Point *getPoint() const;
	const Image *getImage() const;

	void draw() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->img; }
