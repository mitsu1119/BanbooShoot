#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Util.h"
#include "MovingPath.h"

// Base class.
class Character {
protected:
	Animation *anim;

	Point *point;				/* central coordinate */

	// Parameters.
	double speed;

public:
	Character(const Image *img, int initX, int initY, double speed);																/* generate from image handle */
	Character(std::vector<const Image *> &anim, size_t animInterval, int initX, int initY, double speed);	/* generate from animation parts */
	~Character();

	const Point *getPoint() const;
	void setCoord(double x, double y);

	const Image *getImage() const;

	virtual void update();
	virtual void draw() const;
};
inline const Point *Character::getPoint() const { return this->point; }
inline const Image *Character::getImage() const { return this->anim->getImage(); }

// Player class.
class Player: public Character {
private:
	Animation *leftAnim, *rightAnim;
	Direction movingDir;

public:
	Player(const Image *img, int initX, int initY, double speed);																/* generate from a image handle */
	Player(std::vector<const Image *> &anim, std::vector<const Image *> &leftAnimation, std::vector<const Image *> &rightAnimation, size_t animInterval, int initX, int initY, double speed);	/* generate from animation parts */
	~Player();

	void move(Direction dir);
	void draw() const;
};

// Enemy class.
class Enemy: public Character {
private:

public:
	Enemy(const Image *img, int initX, int initY, double speed);															/* generate from a image handle */
	Enemy(std::vector<const Image *> &anim, size_t animInterval, int initX, int initY, double speed);		/* generate from animation parts */
};
