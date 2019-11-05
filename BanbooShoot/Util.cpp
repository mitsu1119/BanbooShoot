#include "Util.h"

// ----------------------------------------------------- Point class ------------------------------------------------------
Point::Point(): x(0), y(0) {
}

Point::Point(double x, double y): x(x), y(y) {
}

// ----------------------------------------------------- Image class ------------------------------------------------------
Image::Image(const char *path) {
	this->handle = LoadGraph(path);
	GetGraphSize(this->handle, &this->sizeX, &this->sizeY);
}
