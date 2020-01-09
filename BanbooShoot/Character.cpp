#include "Character.h"

// ----------------------------------------------------- Character class --------------------------------------------------
Character::Character(const Image *img, int initX, int initY, double speed): speed(speed) {
	this->point = new Point(initX, initY);
	this->anim = new Animation(img);
}

Character::Character(std::vector<const Image *> &anim, size_t animInterval, int initX, int initY, double speed): speed(speed) {
	this->point = new Point(initX, initY);
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
Player::Player(const Image *img, int initX, int initY, double speed): Character(img, initX, initY, speed), movingDir(CENTER) {
	this->leftAnim = new Animation(img);
	this->rightAnim = new Animation(img);
}

Player::Player(std::vector<const Image *> &anim, std::vector<const Image *> &leftAnimation, std::vector<const Image *> &rightAnimation, size_t animInterval, int initX, int initY, double speed): Character(anim, animInterval, initX, initY, speed), movingDir(CENTER) {
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
Enemy::Enemy(const Image *img, int initX, int initY, double speed, MovingPath &&movingPath): Character(img, initX, initY, speed), mpath(movingPath), t(0), segNum(0), segS(initX, initY), init(initX, initY) {
	if(this->mpath.size() >= 2) this->segE = this->mpath[1].getLine() + this->init;
	else this->segE = this->segS;
}

Enemy::Enemy(std::vector<const Image *> &anim, size_t animInterval, int initX, int initY, double speed, MovingPath &&movingPath): Character(anim, animInterval, initX, initY, speed), mpath(movingPath), t(0), segNum(0), segS(initX, initY), init(initX, initY) {
	if(this->mpath.size() >= 2) this->segE = this->mpath[1].getLine() + this->init;
	else this->segE = this->segS;
}

void Enemy::update() {
	Character::update();
	move();
}

void Enemy::move() {
	if(this->t >= 1) {
		segNum++;
		this->t = 0;
		if(mpath.size() >= this->segNum + 2) {
			this->segS = this->segE;			// this->segE == this->mpath[segNum].getLine() + this->init
			this->segE = this->mpath[segNum + 1].getLine() + this->init;
		} else {
			this->segS = this->segE;
		}
	}

	this->point->setX(this->segS.getX() + (segE.getX() - segS.getX()) * t);
	this->point->setY(this->segS.getY() + (segE.getY() - segS.getY()) *t);

	this->t += 1.0 / 60.0;
}

void Enemy::draw() const {
	const Point *pt, *pt2 = nullptr;
	bool isFirst = true;
	for(auto &&node: this->mpath) {
		pt = &node.getLine();
		DrawCircle(this->init.getX() + pt->getX(), this->init.getY() + pt->getY(), 3, WHITE);
		if(!isFirst) DrawLine(this->init.getX() + pt->getX(), this->init.getY() +  pt->getY(), this->init.getX() + pt2->getX(), this->init.getY() + pt2->getY(), WHITE);
		pt2 = pt;
		isFirst = false;
	}
	Character::draw();
}
