#include "MovingPath.h"

MovingPathNode::MovingPathNode(Point &linePoint): type(MPNT_LINE) {
	this->line = new Point(linePoint.getX(), linePoint.getY());
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
	if(this->type == MPNT_LINE) delete this->line;
	else if(this->type == MPNT_BEZIER) delete this->bezier;
}
