#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Util.h"

// Base class.
class Character {
protected:
	Animation *anim;

	Point *point;				/* central coordinate */

	// Parameters.
	double speed;

public:
	Character(const Image *img, double speed);																/* generate from image handle */
	Character(std::vector<const Image *> &anim, size_t animInterval, double speed);	/* generate from animation parts */
	~Character();

	const Point *getPoint() const;
	void setCoord(double x, double y);

	const Image *getImage() const;

	virtual void update();
	void draw() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->anim->getImage(); }

// Player class.
class Player: public Character {
public:
	Player(const Image *img, double speed);																/* generate from image handle */
	Player(std::vector<const Image *> &anim, size_t animInterval, double speed);	/* generate from animation parts */

	void move(Direction dir);
};
