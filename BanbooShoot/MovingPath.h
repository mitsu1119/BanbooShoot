#pragma once
#include <vector>
#include <cmath>
#include "Util.h"

// キャラクタの移動情報のタイプ
// MPNT_LINE: 直線
// MPNT_BEZIER: ベジェ曲線
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3, node4;
	double length;
	size_t segNum;
} BezierNode;

// "bz" で表されるベジェ曲線のパラメータ "t" における座標を計算
Point calcBezierPoint(double t, const BezierNode &bz);

typedef struct _LineNode {
	Point snode, enode;
	double length;
	size_t segNum;
} LineNode;

typedef union _MPNode {
	LineNode *line;
	BezierNode *bezier;
} MPNode;

// キャラクタの移動パスのセグメント
class MovingPathNode {
private:
	MovingPathNodeType type;
	MPNode node;

public:
	MovingPathNode(Point &&startPoint, Point &&endPoint, size_t segNum);
	MovingPathNode(Point &&bezir1, Point &&bezir2, Point &&bezir3, Point &&bezir4, size_t segNum);
	MovingPathNode(const MovingPathNode &movingpathnode) = default;
	MovingPathNode(MovingPathNode &&movingpathnode) noexcept;
	~MovingPathNode();

	const MPNode &getNode() const;
	MovingPathNodeType getType() const;
};

class MovingPath {
private:
	// 連続した曲線のリスト
	/* paths = paths[0] if 0 <= t < 1
	 *				paths[1] if 1 <= t < 2
	 *				......
	 *				paths[i] if i <= t < i + 1
	 *				......
	 * ただし paths[i](i + 1) == paths[i + 1](i + 1)
	 * このようにして一つの関数とみているため、本来ベジェ曲線のパラメータ t は 0 <= t <= 1 の範囲であるがここでのベジェ曲線の実装はセグメントにおける番号も考慮した実装となる
	 * 例えば paths[1] のベジェ曲線は、本来 f(t) であるところを f(t - 1) としている(余分な t のパディングを引いている)
	 */
	std::vector<MovingPathNode> paths;

	// 各セグメントの弧長の累積和
	std::vector<double> cumsum;

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
