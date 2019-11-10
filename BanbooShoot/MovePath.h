#pragma once
#include <vector>
#include "Util.h"

// Moving path information.
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BESIER
};

typedef struct _BezierNode {
	Point node1, node2, node3;
} BezierNode;

class MovingPathNode {
private:
	MovingPathNodeType type;
	Point *line;
	BezierNode *bezir;

public:
	MovingPathNode(Point linePoint);
	MovingPathNode(Point bezir1, Point bezir2, Point bezir3);
	~MovingPathNode();
};
typedef std::vector<MovingPathNode> MovingPath;
