#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Util.h"

// Base class.
class Character {
protected:
	const Animation *anim;
	bool imgFreeFlag;		/* If @img was allcated in the constructor, @img must freed in the destructor */

	Point *point;				/* central coordinate */

	// Parameters.
	double speed;

public:
	Character(const Image *img, double speed);			/* generate from image handle */
	~Character();

	const Point *getPoint() const;
	void setCoord(double x, double y);

	const Image *getImage() const;

	void draw() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->anim->getImage(); }

// Player class.
class Player: public Character {
public:
	Player(const Image *img, double speed);			/* generate from image handle */

	void move(Direction dir);
};
