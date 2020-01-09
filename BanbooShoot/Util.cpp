#include "Util.h"

// ------------------------------------------------------------------------------------------------------------------------
std::vector<std::string> splitStr(const std::string &str, char delim) {
	std::vector<std::string> elems;
	std::string x;
	for(auto ch: str) {
		if(ch == delim) {
			if(!x.empty()) elems.emplace_back(x);
			x.clear();
		} else {
			x += ch;
		}
	}

	if(!x.empty()) elems.emplace_back(x);
	return elems;
}

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

Image::Image(const char *path, int srcX, int srcY, int width, int height) {
	int bufHandle = LoadGraph(path);
	this->handle = DerivationGraph(srcX, srcY, width, height, bufHandle);
	this->sizeX = width;
	this->sizeY = height;
}
