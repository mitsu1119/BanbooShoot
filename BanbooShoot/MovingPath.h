#pragma once
#include <vector>
#include <cmath>
#include <cassert>
#include "Util.h"

// キャラクタの移動情報のタイプ
// MPNT_LINE: 直線
// MPNT_BEZIER: ベジェ曲線
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3, node4;
} BezierNode;

// "bz" で表されるベジェ曲線のパラメータ "t" における座標を計算
Point calcBezierPoint(double t, const BezierNode &bz);

// "bz" で表されるベジェ曲線のパラメータ "t" における微分係数を計算
// 戻り値は Point(dx/dt, dy/dt)
Point calcDerivateBezier(double t, const BezierNode &bz);

typedef struct _LineNode {
	Point snode, enode;
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
	// 連続した曲線のリスト
	/* paths = paths[0] if t = 0
	 *				paths[1] if 0 < t <= 1
	 *				......
	 *				paths[i] if i - 1 < t <= i
	 *				......
	 * ただし paths[i](i) == paths[i + 1](i)
	 * このようにして一つの関数とみているため、本来ベジェ曲線のパラメータ t は 0 <= t <= 1 の範囲であるがここでのベジェ曲線の実装はセグメントにおける番号も考慮した実装となる
	 * 例えば paths[2] のベジェ曲線は、本来 f(t) であるところを f(t - 1) としている(余分な t のパディングを引いている)
	 */
	std::vector<MovingPathNode> paths;

	// 各ノードでの t パラメータの終わりの値
	std::vector<double> time;

	// t パラメータの始まりと終わり
	double tmin, tmax;

	// パスの各セグメントの長さとその累積和
	std::vector<double> lengthSeg, cumsumLenSeg;
	double lengthTotal;

	// パスの中のパラメータ @t に対応する座標を返す関数
	Point calcPathDerivate(double t);		// 微分係数版

	// @i 番のセグメントの始点から @t までの弧長
	// i == 0 だとエラー
	double arcLength(size_t i, double t);

	// パスに沿って始点から @s だけ進んだ時、その地点に対応するパスのパラメータ t を計算
public:
	MovingPath();
	MovingPath(std::string path);

	Point calcPathPoint(double t);
	double getParam(double s);

	using iterator = std::vector<MovingPathNode>::iterator;
	using const_iterator = std::vector<MovingPathNode>::const_iterator;

	size_t size() { return this->paths.size(); }
	iterator begin() { return this->paths.begin(); }
	iterator end() { return this->paths.end(); }
	const_iterator begin() const { return this->paths.begin(); }
	const_iterator end() const { return this->paths.end(); }

	MovingPathNode &operator [](size_t n) { return this->paths[n]; }
};
