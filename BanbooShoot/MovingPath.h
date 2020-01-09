#pragma once
#include <vector>
#include "Util.h"

// Moving path information.
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3;
} BezierNode;

class MovingPathNode {
private:
	MovingPathNodeType type;
	Point line;
	BezierNode *bezier;

public:
	MovingPathNode(Point &&linePoint);
	MovingPathNode(Point &bezir1, Point &bezir2, Point &bezir3);
	~MovingPathNode();
};

class MovingPath {
private:
	std::vector<MovingPathNode> paths;

public:
	MovingPath();
	MovingPath(std::string path);
};
