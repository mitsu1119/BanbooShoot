#include "MovingPath.h"

MovingPathNode::MovingPathNode(Point &&linePoint): type(MPNT_LINE) {
	this->line = std::move(linePoint);
}

MovingPathNode::MovingPathNode(Point &bezir1, Point &bezir2, Point &bezir3) : type(MPNT_BEZIER) {
	this->bezier = new BezierNode;
	this->bezier->node1.setX(bezir1.getX());
	this->bezier->node1.setY(bezir1.getY());
	this->bezier->node2.setX(bezir2.getX());
	this->bezier->node2.setY(bezir2.getY());
	this->bezier->node3.setX(bezir3.getX());
	this->bezier->node3.setY(bezir3.getY());
}

MovingPathNode::~MovingPathNode() {
}

MovingPath::MovingPath() {
}

MovingPath::MovingPath(std::string path) {
	int x, y;
	auto tokens = splitStr(path, ' ');
	int now = MPNT_LINE;
	for(size_t i = 0; i < tokens.size(); ++i) {
		if(tokens[i] == "l") {
			now = MPNT_LINE;
			continue;
		}

		x = std::stoi(tokens[i]);
		y = std::stoi(tokens[i+1]);
		i++;
		if(now == MPNT_LINE) this->paths.emplace_back(MovingPathNode(Point(x, y)));
	}
}
