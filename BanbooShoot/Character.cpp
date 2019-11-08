#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const Image *img, double speed): speed(speed) {
	this->point = new Point(0, 0);
	this->anim = new Animation(img);
}

Character::Character(std::vector<const Image *> &anim, size_t animInterval, double speed): speed(speed) {
	this->point = new Point(0, 0);
	this->anim = new Animation(anim, animInterval);
}

Character::~Character() {
	delete this->point;
	delete this->anim;
}

void Character::setCoord(double x, double y) {
	this->point->setX(x);
	this->point->setY(y);
}

void Character::update() {
	this->anim->update();
}

void Character::draw() const {
	DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->anim->getHandle(), true);
}

// ----------------------------------------------------- Player class -----------------------------------------------------
Player::Player(const Image *img, double speed): Character(img, speed), movingDir(CENTER) {
	this->leftAnim = new Animation(img);
	this->rightAnim = new Animation(img);
}

Player::Player(std::vector<const Image *> &anim, std::vector<const Image *> &leftAnimation, std::vector<const Image *> &rightAnimation, size_t animInterval, double speed): Character(anim, animInterval, speed), movingDir(CENTER) {
	this->leftAnim = new Animation(leftAnimation, animInterval);
	this->rightAnim = new Animation(rightAnimation, animInterval);
}

Player::~Player() {
	delete this->leftAnim;
	delete this->rightAnim;
}

void Player::move(Direction dir) {
	this->movingDir = dir;
	if(dir == CENTER) return;

	this->point->moveX(speed * cos(dir *M_PI / 4));
	this->point->moveY(-1 * speed * sin(dir * M_PI / 4));
}

void Player::draw() const {
	switch(this->movingDir) {
	case RUP:
	case RIGHT:
	case RDOWN:
		DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->rightAnim->getHandle(), true);
		break;
	case LUP:
	case LEFT:
	case LDOWN:
		DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->leftAnim->getHandle(), true);
		break;
	default:
		DrawRotaGraph((int)this->point->getX(), (int)this->point->getY(), 1.0, 0.0, this->anim->getHandle(), true);
	}
}

// ----------------------------------------------------- Enemy class ------------------------------------------------------
Enemy::Enemy(const Image *img, double speed) : Character(img, speed) {
}

Enemy::Enemy(std::vector<const Image *> &anim, size_t animInterval, double speed): Character(anim, animInterval, speed) {
}
