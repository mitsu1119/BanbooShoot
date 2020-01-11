#pragma once
#include <vector>
#include <cmath>
#include "Util.h"

// Moving path information.
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3, node4;
	double length;
} BezierNode;

Point calcBezierPoint(double t, const BezierNode &bz);

typedef struct _LineNode {
	Point snode, enode;
	double length;
} LineNode;

typedef union _MPNode {
	LineNode *line;
	BezierNode *bezier;
} MPNode;

class MovingPathNode {
private:
	MovingPathNodeType type;
	MPNode node;

public:
	MovingPathNode(Point &&startPoint, Point &&endPoint);
	MovingPathNode(Point &&bezir1, Point &&bezir2, Point &&bezir3, Point &&bezir4);
	MovingPathNode(const MovingPathNode &movingpathnode) = default;
	MovingPathNode(MovingPathNode &&movingpathnode) noexcept;
	~MovingPathNode();

	const MPNode &getNode() const;
	MovingPathNodeType getType() const;
};

class MovingPath {
private:
	std::vector<MovingPathNode> paths;

public:
	MovingPath();
	MovingPath(std::string path);

	using iterator = std::vector<MovingPathNode>::iterator;
	using const_iterator = std::vector<MovingPathNode>::const_iterator;

	size_t size() { return this->paths.size(); }
	iterator begin() { return this->paths.begin(); }
	iterator end() { return this->paths.end(); }
	const_iterator begin() const { return this->paths.begin(); }
	const_iterator end() const { return this->paths.end(); }

	MovingPathNode &operator [](size_t n) { return this->paths[n]; }
};
