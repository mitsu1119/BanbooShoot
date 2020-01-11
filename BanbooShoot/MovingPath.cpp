#include "MovingPath.h"

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
}

MovingPath::MovingPath(std::string path) {
	double x, y;
	auto tokens = splitStr(path, {' ', ','});
	bool firstFlag = true;
	Point *pad = nullptr;
	int now = MPNT_LINE;
	size_t cnt = 0;
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
				this->paths.emplace_back(start - *pad, end - *pad);
				start = end;
				cnt++;
			} else {
				x = std::stof(tokens[cnt + 4]);
				y = std::stof(tokens[cnt + 5]);
				Point end(x, y);
				this->paths.emplace_back(start - *pad, Point(std::stof(tokens[cnt]), std::stof(tokens[cnt + 1])) - *pad, Point(std::stof(tokens[cnt + 2]), std::stof(tokens[cnt + 3])) - *pad, end - *pad);
				start = end;
				cnt += 5;
			}
		}
		cnt++;
	}

	if(pad != nullptr) delete pad;
}
