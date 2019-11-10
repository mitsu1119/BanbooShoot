#include "MovingPath.h"

MovingPathNode::MovingPathNode(Point &linePoint): type(MPNT_LINE) {
	this->line = new Point(linePoint.getX(), linePoint.getY());
}

MovingPathNode::MovingPathNode(Point &bezir1, Point &bezir2, Point &bezir3): type(MPNT_BEZIER) {
	this->bezir = new BezierNode;
	this->bezir.node1.setX(bezir1.getX());
	this->bezir.node1.setY(bezir1.getY());
	this->bezir.node2.setX(bezir2.getX());
	this->bezir.node2.setY(bezir2.getY());
	this->bezir.node3.setX(bezir3.getX());
	this->bezir.node3.setY(bezir3.getY());
}

MovingPathNode::~MovingPathNode() {
	if(this->type == MPNT_LINE) delete this->line;
	else delete this->bezir;
}
