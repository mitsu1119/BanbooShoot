#pragma once
#include <vector>
#include <cmath>
#include <cassert>
#include "Util.h"

// �L�����N�^�̈ړ����̃^�C�v
// MPNT_LINE: ����
// MPNT_BEZIER: �x�W�F�Ȑ�
enum MovingPathNodeType {
	MPNT_LINE, MPNT_BEZIER
};

typedef struct _BezierNode {
	Point node1, node2, node3, node4;
} BezierNode;

// "bz" �ŕ\�����x�W�F�Ȑ��̃p�����[�^ "t" �ɂ�������W���v�Z
Point calcBezierPoint(double t, const BezierNode &bz);

// "bz" �ŕ\�����x�W�F�Ȑ��̃p�����[�^ "t" �ɂ���������W�����v�Z
// �߂�l�� Point(dx/dt, dy/dt)
Point calcDerivateBezier(double t, const BezierNode &bz);

typedef struct _LineNode {
	Point snode, enode;
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
	// �A�������Ȑ��̃��X�g
	/* paths = paths[0] if t = 0
	 *				paths[1] if 0 < t <= 1
	 *				......
	 *				paths[i] if i - 1 < t <= i
	 *				......
	 * ������ paths[i](i) == paths[i + 1](i)
	 * ���̂悤�ɂ��Ĉ�̊֐��Ƃ݂Ă��邽�߁A�{���x�W�F�Ȑ��̃p�����[�^ t �� 0 <= t <= 1 �͈̔͂ł��邪�����ł̃x�W�F�Ȑ��̎����̓Z�O�����g�ɂ�����ԍ����l�����������ƂȂ�
	 * �Ⴆ�� paths[2] �̃x�W�F�Ȑ��́A�{�� f(t) �ł���Ƃ���� f(t - 1) �Ƃ��Ă���(�]���� t �̃p�f�B���O�������Ă���)
	 */
	std::vector<MovingPathNode> paths;

	// �e�m�[�h�ł� t �p�����[�^�̏I���̒l
	std::vector<double> time;

	// t �p�����[�^�̎n�܂�ƏI���
	double tmin, tmax;

	// �p�X�̊e�Z�O�����g�̒����Ƃ��̗ݐϘa
	std::vector<double> lengthSeg, cumsumLenSeg;
	double lengthTotal;

	// �p�X�̒��̃p�����[�^ @t �ɑΉ�������W��Ԃ��֐�
	Point calcPathDerivate(double t);		// �����W����

	// @i �Ԃ̃Z�O�����g�̎n�_���� @t �܂ł̌ʒ�
	// i == 0 ���ƃG���[
	double arcLength(size_t i, double t);

	// �p�X�ɉ����Ďn�_���� @s �����i�񂾎��A���̒n�_�ɑΉ�����p�X�̃p�����[�^ t ���v�Z
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
