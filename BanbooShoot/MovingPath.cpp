#include "MovingPath.h"

Point calcBezierPoint(double t, const BezierNode &bz) {
	t -= bz.segNum;
	if(t < 0) {
		t = 10;
	}
	double x = (1 - t) * (1 - t) * (1 - t) * bz.node1.getX() + 3 * (1 - t) * (1 - t) * t * bz.node2.getX() + 3 * (1 - t) * t * t * bz.node3.getX() + t * t * t * bz.node4.getX();
	double y = (1 - t) * (1 - t) * (1 - t) * bz.node1.getY() + 3 * (1 - t) * (1 - t) * t * bz.node2.getY() + 3 * (1 - t) * t * t * bz.node3.getY() + t * t * t * bz.node4.getY();
	return Point(x, y);
}

MovingPathNode::MovingPathNode(Point &&startPoint, Point &&endPoint, size_t segNum): type(MPNT_LINE) {
	this->node.line = new LineNode;
	this->node.line->snode = std::move(startPoint);
	this->node.line->enode = std::move(endPoint);
	this->node.line->length = std::sqrt(std::pow(this->node.line->enode.getX() - this->node.line->snode.getX(), 2) + std::pow(this->node.line->enode.getY() - this->node.line->snode.getY(), 2));
	this->node.line->segNum = segNum;
}

MovingPathNode::~MovingPathNode() {
	if(this->type == MPNT_LINE) delete this->node.line;
	else delete this->node.bezier;
}

MovingPathNode::MovingPathNode(Point &&bezir1, Point &&bezir2, Point &&bezir3, Point &&bezir4, size_t segNum) : type(MPNT_BEZIER) {
	this->node.bezier = new BezierNode;
	this->node.bezier->node1 = std::move(bezir1);
	this->node.bezier->node2 = std::move(bezir2);
	this->node.bezier->node3 = std::move(bezir3);
	this->node.bezier->node4 = std::move(bezir4);
	this->node.bezier->segNum = segNum;

	// Calc bezier length.
	size_t accurate = 10;
	double length = 0.0, t;
	Point prev;
	for(size_t i = 0; i <= accurate; i++) {
		t = segNum + (double)i / (double)accurate;
		Point pt = calcBezierPoint(t, *this->node.bezier);
		if(i > 0) {
			double dx = pt.getX() - prev.getX();
			double dy = pt.getY() - prev.getY();
			length += std::sqrt(dx * dx + dy * dy);
		}
		prev = pt;
	}
	this->node.bezier->length = length;
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
}

MovingPath::MovingPath(std::string path) {
	double x, y, cumsumbuf = 0;
	auto tokens = splitStr(path, {' ', ','});
	bool firstFlag = true;
	Point *pad = nullptr;
	int now = MPNT_LINE;
	size_t cnt = 0, index = 0;
	Point start;
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
			if(now == MPNT_LINE) {
				x = std::stof(tokens[cnt]);
				y = std::stof(tokens[cnt + 1]);
				Point end(x, y);
				this->paths.emplace_back(start - *pad, end - *pad, index);
				cumsumbuf += this->paths.back().getNode().line->length;
				this->cumsum.emplace_back(cumsumbuf);
				start = end;
				index++;
				cnt++;
			} else {
				x = std::stof(tokens[cnt + 4]);
				y = std::stof(tokens[cnt + 5]);
				Point end(x, y);
				this->paths.emplace_back(start - *pad, Point(std::stof(tokens[cnt]), std::stof(tokens[cnt + 1])) - *pad, Point(std::stof(tokens[cnt + 2]), std::stof(tokens[cnt + 3])) - *pad, end - *pad, index);
				cumsumbuf += this->paths.back().getNode().bezier->length;
				this->cumsum.emplace_back(cumsumbuf);
				start = end;
				index++;
				cnt += 5;
			}
		}
		cnt++;
	}

	if(pad != nullptr) delete pad;
}
