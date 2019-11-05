#include "Util.h"

// ----------------------------------------------------- Point class ------------------------------------------------------
Point::Point(): x(0), y(0) {
}

Point::Point(double x, double y): x(x), y(y) {
}

// ----------------------------------------------------- ScreenRect class -------------------------------------------------
ScreenRect::ScreenRect(): leftX(0), topY(0), rightX(0), bottomY(0) {
}

ScreenRect::ScreenRect(int leftX, int topY, int rightX, int bottomY): leftX(leftX), topY(topY), rightX(rightX), bottomY(bottomY) {
}

// ----------------------------------------------------- Image class ------------------------------------------------------
Image::Image(const char *path) {
	this->handle = LoadGraph(path);
	GetGraphSize(this->handle, &this->sizeX, &this->sizeY);
}
