#pragma once
#define _USE_MATH_DEFINES
#include "Util.h"
#include <cmath>

// Base class.
class Character {
protected:
	const Image *img;
	Point *point;				/* central coordinate */

public:
	Character(const char *imgpath);		/* generate from image file path */
	~Character();

	const Point *getPoint() const;
	void setCoord(double x, double y);

	const Image *getImage() const;

	void draw() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->img; }

// Player class.
class Player: public Character {
public:
	Player(const char *imgpath);		/* generate from image file path */

	void move(Direction dir);
};
