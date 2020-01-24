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
Enemy::Enemy(const Image *img, int initX, int initY, double speed, MovingPath &&movingPath): Character(img, initX, initY, speed), mpath(movingPath), t(0), segNum(0), init(initX, initY) {
}

Enemy::Enemy(std::vector<const Image *> &anim, size_t animInterval, int initX, int initY, double speed, MovingPath &&movingPath): Character(anim, animInterval, initX, initY, speed), mpath(movingPath), t(0),  segNum(0), init(initX, initY) {
}

void Enemy::update() {
	Character::update();
	move();
}

void Enemy::move() {
	Point pt = this->mpath.calcPathPoint(this->mpath.getParam(t));
	this->point->setX(this->init.getX() + pt.getX());
	this->point->setY(this->init.getY() + pt.getY());
	this->t += 5;
}

void Enemy::draw() const {
	const LineNode *li;
	const BezierNode *bz;
	const MPNode *dnode;
	double x, y;
	bool isFirst = true;

	Character::draw();
	for(auto &&node: this->mpath) {
		dnode = &node.getNode();
		if(node.getType() == MPNT_LINE) {
			li = dnode->line;
			if(isFirst) DrawCircle(this->init.getX() + li->snode.getX(), this->init.getY() +li->snode.getY(), 3, WHITE);
			DrawCircle(this->init.getX() + li->enode.getX(), this->init.getY() +li->enode.getY(), 3, WHITE);
			DrawLine(this->init.getX() + li->snode.getX(), this->init.getY() + li->snode.getY(), this->init.getX() + li->enode.getX(), this->init.getY() + li->enode.getY(), WHITE);
			isFirst = false;
		} else {
			bz = dnode->bezier;
			if(isFirst) DrawCircle(this->init.getX() + bz->node1.getX(), this->init.getY() +bz->node1.getY(), 3, WHITE);
			DrawCircle(this->init.getX() + bz->node4.getX(), this->init.getY() +bz->node4.getY(), 3, WHITE);

			// Draw bezier
			for(double t = 0; t <= 1; t += 0.01) {
				x = (1 - t) * (1 - t) * (1 - t) * bz->node1.getX() + 3 * (1 - t) * (1 - t) * t * bz->node2.getX() + 3 * (1 - t) * t * t * bz->node3.getX() + t * t * t * bz->node4.getX();
				y = (1 - t) * (1 - t) * (1 - t) * bz->node1.getY() + 3 * (1 - t) * (1 - t) * t * bz->node2.getY() + 3 * (1 - t) * t * t * bz->node3.getY() + t * t * t * bz->node4.getY();
				DrawPixel(this->init.getX() + x, this->init.getY() + y, WHITE);
			}
			isFirst = false;
		}
	}
}
