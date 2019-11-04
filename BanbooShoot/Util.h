#pragma once
#include <vector>
#include <string>
#include "DxLib.h"

// 2 dimention coordinate
class Point {
private:
	double x, y;

public:
	Point();
	Point(double x, double y);

	double getX() const;
	double getY() const;
};
inline double Point::getX() const { return this->x; }
inline double Point::getY() const { return this->y; }

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
