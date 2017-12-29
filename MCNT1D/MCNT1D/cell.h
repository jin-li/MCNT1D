#pragma once
#ifndef CELL_H
#define CELL_H

#include "material.h"

class cell {
public:
	double left;    //դԪ��߽�
	double right;   //դԪ�ұ߽�
	material mat;   //դԪ����

	//ʹ��Ĭ�ϵĹ��캯��
	cell() = default;

	//���캯��
	cell(double _left, double _right, material _mat) :left(_left), right(_right), mat(_mat) {}

	//���ƹ��캯��
	cell(const cell &_c) {
		left = _c.left;
		right = _c.right;
		mat = _c.mat;
	}
};

#endif // !CELL_H
