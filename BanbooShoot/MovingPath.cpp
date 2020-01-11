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

MovingPathNode::MovingPathNode(Point &bezir1, Point &bezir2, Point &bezir3) : type(MPNT_BEZIER) {
	this->node.bezier = new BezierNode;
	this->node.bezier->node1.setX(bezir1.getX());
	this->node.bezier->node1.setY(bezir1.getY());
	this->node.bezier->node2.setX(bezir2.getX());
	this->node.bezier->node2.setY(bezir2.getY());
	this->node.bezier->node3.setX(bezir3.getX());
	this->node.bezier->node3.setY(bezir3.getY());
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

	size_t cnt = 0;
	while(cnt < tokens.size()) {
		if(tokens[cnt] == "m") {
			x = std::stof(tokens[cnt + 1]);
			y = std::stof(tokens[cnt + 2]);
			cnt += 3;
			if(pad == nullptr) pad = new Point(x, y);

			Point end(x, y);
			while(cnt < tokens.size() && std::isdigit(tokens[cnt][0])) {
				Point start = end;
				x = std::stof(tokens[cnt]);
				y = std::stof(tokens[cnt + 1]);
				end = Point(x, y);

				this->paths.emplace_back(start - *pad, end - *pad);
				cnt += 2;
			}
		}
	}
	if(pad != nullptr) delete pad;
}
