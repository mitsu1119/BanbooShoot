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

	const Point &getLine() const;
};

class MovingPath {
private:
	std::vector<MovingPathNode> paths;

public:
	MovingPath();
	MovingPath(int initX, int initY, std::string path);

	using iterator = std::vector<MovingPathNode>::iterator;
	using const_iterator = std::vector<MovingPathNode>::const_iterator;

	size_t size() { return this->paths.size(); }
	iterator begin() { return this->paths.begin(); }
	iterator end() { return this->paths.end(); }
	const_iterator begin() const { return this->paths.begin(); }
	const_iterator end() const { return this->paths.end(); }

	MovingPathNode &operator [](size_t n) { return this->paths[n]; }
};
