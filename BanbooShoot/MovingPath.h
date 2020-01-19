#pragma once
#include <vector>
#include <cmath>
#include "Util.h"

// �L�����N�^�̈ړ����̃^�C�v
// MPNT_LINE: ����
// MPNT_BEZIER: �x�W�F�Ȑ�
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3, node4;
	double length;
	size_t segNum;
} BezierNode;

// "bz" �ŕ\�����x�W�F�Ȑ��̃p�����[�^ "t" �ɂ�������W���v�Z
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

// �L�����N�^�̈ړ��p�X�̃Z�O�����g
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
	// �A�������Ȑ��̃��X�g
	/* paths = paths[0] if 0 <= t < 1
	 *				paths[1] if 1 <= t < 2
	 *				......
	 *				paths[i] if i <= t < i + 1
	 *				......
	 * ������ paths[i](i + 1) == paths[i + 1](i + 1)
	 * ���̂悤�ɂ��Ĉ�̊֐��Ƃ݂Ă��邽�߁A�{���x�W�F�Ȑ��̃p�����[�^ t �� 0 <= t <= 1 �͈̔͂ł��邪�����ł̃x�W�F�Ȑ��̎����̓Z�O�����g�ɂ�����ԍ����l�����������ƂȂ�
	 * �Ⴆ�� paths[1] �̃x�W�F�Ȑ��́A�{�� f(t) �ł���Ƃ���� f(t - 1) �Ƃ��Ă���(�]���� t �̃p�f�B���O�������Ă���)
	 */
	std::vector<MovingPathNode> paths;

	// �e�Z�O�����g�̌ʒ��̗ݐϘa
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
