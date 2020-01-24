#include "MovingPath.h"

Point calcBezierPoint(double t, const BezierNode &bz) {
	double x = (1 - t) * (1 - t) * (1 - t) * bz.node1.getX() + 3 * (1 - t) * (1 - t) * t * bz.node2.getX() + 3 * (1 - t) * t * t * bz.node3.getX() + t * t * t * bz.node4.getX();
	double y = (1 - t) * (1 - t) * (1 - t) * bz.node1.getY() + 3 * (1 - t) * (1 - t) * t * bz.node2.getY() + 3 * (1 - t) * t * t * bz.node3.getY() + t * t * t * bz.node4.getY();
	return Point(x, y);
}

Point calcDerivateBezier(double t, const BezierNode &bz) {
	double a = bz.node1.getX(), b = bz.node2.getX(), c = bz.node3.getX(), d = bz.node4.getX();
	double x = 3 * (-a + 3 * b - 3 * c + d) * t * t + 6 * (a - 2 * b + c) * t + 3 * (-a + b);

	a = bz.node1.getY(), b = bz.node2.getY(), c = bz.node3.getY(), d = bz.node4.getY();
	double y = 3 * (-a + 3 * b - 3 * c + d) * t * t + 6 * (a - 2 * b + c) * t + 3 * (-a + b);
	
	return Point(x, y);
}

MovingPathNode::MovingPathNode(Point &&startPoint, Point &&endPoint): type(MPNT_LINE) {
	this->node.line = new LineNode;
	this->node.line->snode = std::move(startPoint);
	this->node.line->enode = std::move(endPoint);
}

MovingPathNode::~MovingPathNode() {
	if(this->type == MPNT_LINE) delete this->node.line;
	else delete this->node.bezier;
}

MovingPathNode::MovingPathNode(Point &&bezir1, Point &&bezir2, Point &&bezir3, Point &&bezir4) : type(MPNT_BEZIER) {
	this->node.bezier = new BezierNode;
	this->node.bezier->node1 = std::move(bezir1);
	this->node.bezier->node2 = std::move(bezir2);
	this->node.bezier->node3 = std::move(bezir3);
	this->node.bezier->node4 = std::move(bezir4);
}

MovingPathNode::MovingPathNode(MovingPathNode &&movingpathnode) noexcept {
	this->type = movingpathnode.type;
	this->node = std::move(movingpathnode.node);
	movingpathnode.node.line = nullptr;
	movingpathnode.node.bezier = nullptr;
}

const MPNode &MovingPathNode::getNode() const {
	return this->node;
}

MovingPathNodeType MovingPathNode::getType() const {
	return this->type;
}

MovingPath::MovingPath() {
	this->paths.emplace_back(MovingPathNode(Point(0, 0), Point(0, 0)));
	this->time = {0.0};
	this->tmin = 0.0;
	this->tmax = 0.0;
	this->lengthSeg = {0.0};
	this->cumsumLenSeg = {0.0};
	this->lengthTotal = 0.0;
}

MovingPath::MovingPath(std::string path) {
	double x, y;
	auto tokens = splitStr(path, {' ', ','});
	bool firstFlag = true;
	Point *pad = nullptr;
	int now = MPNT_LINE;
	size_t cnt = 0, index = 1;
	Point start;

	this->paths.emplace_back(MovingPathNode(Point(0, 0), Point(0, 0)));
	this->time = {0.0};
	while(cnt < tokens.size()) {
		if(tokens[cnt] == "M" || tokens[cnt] == "m") {
			now = MPNT_LINE;
			x = std::stof(tokens[cnt + 1]);
			y = std::stof(tokens[cnt + 2]);
			if(pad == nullptr) pad = new Point(x, y);
			start = Point(x, y);
			cnt += 2;
		} else if(tokens[cnt] == "C" || tokens[cnt] == "c") {
			now = MPNT_BEZIER;
		} else if(tokens[cnt] == "L" || tokens[cnt] == "l") {
			now = MPNT_LINE;
		} else {
			assert(pad != nullptr);
			if(now == MPNT_LINE) {
				x = std::stof(tokens[cnt]);
				y = std::stof(tokens[cnt + 1]);
				Point end(x, y);
				this->paths.emplace_back(start - *pad, end - *pad);
				this->time.emplace_back((double)index);
				start = end;
				index++;
				cnt++;
			} else {
				x = std::stof(tokens[cnt + 4]);
				y = std::stof(tokens[cnt + 5]);
				Point end(x, y);
				this->paths.emplace_back(start - *pad, Point(std::stof(tokens[cnt]), std::stof(tokens[cnt + 1])) - *pad, Point(std::stof(tokens[cnt + 2]), std::stof(tokens[cnt + 3])) - *pad, end - *pad);
				this->time.emplace_back((double)index);
				start = end;
				index++;
				cnt += 5;
			}
		}
		cnt++;
	}

	this->tmin = this->time[0];
	this->tmax = this->time.back();

	// 長さの初期化
	this->lengthSeg = std::vector<double>(this->time.size(), 0.0);
	this->cumsumLenSeg = std::vector<double>(this->time.size(), 0.0);
	for(size_t i = 1; i < this->time.size(); i++) {
		this->lengthSeg[i] = arcLength(i, this->time[i]);
		this->cumsumLenSeg[i] = this->cumsumLenSeg[i - 1] + this->lengthSeg[i];
	}
	this->lengthTotal = this->cumsumLenSeg.back();

	if(pad != nullptr) delete pad;
}

Point MovingPath::calcPathPoint(double t) {
	size_t index = 1;

	if(t > this->tmax) t = this->tmax;
	while(t > 1.0) {
		t -= 1.0;
		index++;
	}
	return calcBezierPoint(t, *this->paths[index].getNode().bezier);
}

Point MovingPath::calcPathDerivate(double t) {
	size_t index = 1;
	
	if(t > this->tmax) t = this->tmax;
	while(t > 1.0) {
		t -= 1.0;
		index++;
	}
	return calcDerivateBezier(t, *this->paths[index].getNode().bezier);
}

double MovingPath::arcLength(size_t i, double t) {
	assert(i >0);

	size_t accurate = 10;
	double length = 0.0;
	double a = this->time[i - 1];
	double b = t;
	double dx = (b - a) / (double)accurate;
	double x, y;
	for(size_t j = 0; j < accurate; j++) {
		x = a + (double)j * dx;
		y = x + dx;
		length += ((this->calcPathDerivate(x) + this->calcPathDerivate(y)).getAbs() / 2.0) * dx;
	}

	return length;
}

double MovingPath::getParam(double s) {
	// s \in [0, totalLength] が与えられたときそれに対応する t \in [tmin, tmax] を計算
	if(s <= 0) return this->tmin;
	if(s >= this->lengthTotal) return this->tmax;

	// s がどの番号のセグメントに属するか決定
	size_t i;
	size_t p = this->cumsumLenSeg.size();
	for(i = 1; i < p; i++) {
		if(s < this->cumsumLenSeg[i]) break;
	}

	// ニュートン法の初期化
	double segS = s - this->cumsumLenSeg[i - 1];
	double segL = lengthSeg[i];
	double segTMin = this->time[i - 1];
	double segTMax = this->time[i];
	double segT = segTMin + (segTMax - segTMin) * (segS / segL);

	// 二分法の初期化
	double lower = segTMin, upper = segTMax;

	size_t occurate = 20;
	double eps = 0.5;
	for(size_t j = 0; j < occurate; j++) {
		double F = this->arcLength(i, segT) - segS;
		if(std::abs(F) <= eps) return segT;

		double dfdt = this->calcPathDerivate(segT).getAbs();
		double segTCandidate = segT - F / dfdt;

		if(F > 0) {
			upper = segT;
			if(segTCandidate <= lower) segT = (upper + lower) / 2;
			else segT = segTCandidate;
		} else {
			lower = segT;
			if(segTCandidate >= upper) segT = (upper + lower) / 2;
			else segT = segTCandidate;
		}
	}

	return segT;
}
