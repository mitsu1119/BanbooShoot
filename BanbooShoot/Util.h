#pragma once
#include <vector>
#include <string>
#include "DxLib.h"

// Direction sets.
enum Direction {
	RIGHT, RUP, UP, LUP, LEFT, LDOWN, DOWN, RDOWN, CENTER
};

// 2 dimention coordinate
class Point {
private:
	double x, y;

public:
	Point();
	Point(double x, double y);

	double getX() const;
	double getY() const;
	void moveX(double dx);
	void moveY(double dy);
	void setX(double x);
	void setY(double y);
};
inline double Point::getX() const { return this->x; }
inline double Point::getY() const { return this->y; }
inline void Point::moveX(double dx) { this->x += dx; }
inline void Point::moveY(double dy) { this->y += dy; }
inline void Point::setX(double x) { this->x = x; }
inline void Point::setY(double y) { this->y = y; }

// Screen size set
class ScreenRect {
private:
	int leftX, topY, rightX, bottomY;

public:
	ScreenRect();
	ScreenRect(int leftX, int topY, int rightX, int bottomY);

	int getLeftX() const;
	int getTopY() const;
	int getRightX() const;
	int getBottomY() const;
};
inline int ScreenRect::getLeftX() const { return this->leftX; }
inline int ScreenRect::getTopY() const { return this->topY; }
inline int ScreenRect::getRightX() const { return this->rightX; }
inline int ScreenRect::getBottomY() const { return this->bottomY; }

// General purpose image class
class Image {
private:
	int handle;
	int sizeX, sizeY;

public:
	Image(const char *path);

	int getSizeX() const;
	int getSizeY() const;
	int getHandle() const;
};
inline int Image::getSizeX() const { return this->sizeX; }
inline int Image::getSizeY() const { return this->sizeY; }
inline int Image::getHandle() const { return this->handle; }
